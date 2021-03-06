#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <algorithm>
#include <stdlib.h>
#include <inttypes.h>
#include <bitset>
#include <thread>
#include "distributor.hpp"
#include "biomapper.hpp"

using namespace std;

/*****************************************************************************************
 * Constructor
 ****************************************************************************************/

BioMapper::BioMapper () : chromosomeColumn(1), startColumn(2), endColumn(-1), lastColumn(-1), annotationFileNumber(0),  header(true), threads_to_use(1), zeroBased(false), outputFileName("output.csv"), fileType("csv"), nucleotides(false), maximum_threads(std::thread::hardware_concurrency()), nucleotideColumn(-1), mappingStyle(OVERLAP) { }



/****************************************************************************************
 * Determine important variables
 ***************************************************************************************/

bool BioMapper::determineReferences() {

  std::cout << "Determining references for crossmapping..." << endl << endl;


  for (std::string annotFile : annotationFiles ) {
    std::ifstream annot;
    annot.open(annotFile);
    // Read in all references as a dictionary

    string row;
    std::map <std::string, bool> _refIDs;

    if (header) {
      // remove first line and save for future use
      std::getline(annot, row);
      if (!row.size()) {
	std::cerr << "ERROR: No file size for " << annotFile << ".  Aborting." << std::endl << std::endl;
	return false;
      }
      headerRows.push_back(row);
    }

	char splitter = ',';
	if ( fileType.compare("tsv") == 0 ) {
	    splitter = '\t';
	}
    while ( std::getline(annot, row) ) {
	std::stringstream _rowElements(row);
	std::string _element;
	auto i = 1;

	while ( std::getline(_rowElements, _element, splitter) ) {
		if ( i == chromosomeColumn ) {
	      		//std::map <std::string, bool>::iterator it;
	      		auto it = _refIDs.find(_element);
	      		if ( it == _refIDs.end() ) {
				// New element for this file
				_refIDs[_element] = 1;
				// Element should either have null in it or some number
				// If the element has something in it, increase the number
				// else set it to 1.
				if (referenceIDs[_element]) {
					referenceIDs[_element]++;
				} else {
					referenceIDs[_element] = 1;
				}
	      		}
	      		// If already updated referenceIDs for this _element, ignore and move to next row
	      		// Break out of loop and move to next row
	      		break;
	    	}
		i++;
	}
    }
  }

  // Copy refIDs into threads

  for (auto& refID_threads : referenceIDs) {
	if (refID_threads.second == returnNumberOfAnnotationFiles()) {
	  threads.push_back(refID_threads.first);
	}
  }

  // referenceIDs has all references along with number of occurrences
  // threads vector is set up.
  return true;

}

bool BioMapper::determineArguments(int argc, char** argv) {
	// Determine command line arguments
	bool properArguments = true;
	for (int i = 1; i < argc; i++) {
	  	/**************************************************************
		*
		* Check for --annotations / -a arguments
		* Two or more file need to be provided
		*
		**************************************************************/

		if (strcmp("--annotations", argv[i]) == 0 || strcmp("-a", argv[i]) == 0 ) {
			i++;
			if (i >= argc) {
			    std::cerr << "ERROR: No annotation files following the --annotations/-a flag." << std::endl << std::endl;
			    return false;
			}

			while (strncmp(argv[i], "-", 1) != 0) {
				annotationFiles.push_back(argv[i]);
				std::cout << "Loading file: " << argv[i] << std::endl;
				i++;
				if (i >= argc) {
					i--;
					break;
				}
			}

			//if ( annotationFiles.size() < 2 ) {
			//    std::cerr << "ERROR: Fewer than two files entered.  Please enter at least two files to crossmap."  << std::endl << std::endl;
			//    return false;
			//}
		}

	  	/**************************************************************
		*
		* Check for --output / -o argument
		* This sets the output file name.
		*
		**************************************************************/

		if (strcmp("--output", argv[i]) == 0 || strcmp("-o", argv[i]) == 0 ) {
			i++;
			if (i >= argc) {
				std::cout << "No output file name entered.  Assuming default of 'output.csv' file name." << std::endl << std::endl;
			    std::cerr << "WARNING: No output file name following the --output/-o flag.  File name 'output.csv' used by default." << std::endl << std::endl;
				// return since i is now past the final argument
				return true;
		    } else if ( strncmp(argv[i], "-", 1) == 0 ) {
				std::cout << "Ouput file name flag entered but no parameter provided.  Assuming default of 'output.csv' file name." << std::endl << std::endl;
				std::cerr << "WARNING: Ouput file name flag entered but no parameter provided.  Assuming default of 'output.csv' file name." << std::endl << std::endl;
		    } else {
				//std::string _tmpFileType(argv[i]);
				bool _rtn = setFileName(argv[i]);
				if (!_rtn) {
					// TODO: Make sure that the file name is formatted properly in setFileName function and return false if not to present the following errors
					std::cout << "Ouput file name improperly formatted.  Using default 'output.csv' file name." << std::endl << std::endl;
					std::cerr << "WARNING: Ouput file name improperly formatted.  Using default 'output.csv' file name." << std::endl << std::endl;
				}		    	
		    }
		}


		/**************************************************************
		*
		* Check for --map_type / -m arguments
		* Default is OVERLAP (in both/all files) if this flag isn't set.
		*
		**************************************************************/
		if (strcmp("--map_type", argv[i]) == 0 || strcmp("-m", argv[i]) == 0 ) {
		    i++;
		    if ( i >= argc ) {
			std::cerr << "ERROR: No annotation files following the --map_type/-m flag; annotation flag should not be at the end of the line." << std::endl << std::endl;
			return false;
		    } else if ( strncmp(argv[i], "-", 1) == 0 ) {
			std::cerr << "ERROR: No annotation files following the --map_type/-m flag." << std::endl << std::endl;
			return false;
		    } else {
			std::string _tmpMappingType(argv[i]);
			transform(_tmpMappingType.begin(), _tmpMappingType.end(), _tmpMappingType.begin(), ::tolower);
			if ( _tmpMappingType.compare("overlap") == 0 ) {
			    mappingStyle = OVERLAP;
			    std::cout << "Mapping Style: Overlap (output positions that exist in all entered annotation files." << std::endl;
			} else if ( _tmpMappingType.compare("exclusive") == 0 ) {
			    mappingStyle = EXCLUSIVE;
			} else if ( _tmpMappingType.compare("collapse") == 0 ) {
				mappingStyle = COLLAPSE;	
			} else {
			    std::cerr << "ERROR: Improper type for --map_type/-m.  You must use 'overlap', 'exclusive', or 'collapse' as the flag option." << std::endl << std::endl;
			    return false;
		      }
		    }
		}

		/**************************************************************
		*
		* Check for --chromosome / -c arguments
		* Default is column 1 if this flag isn't set.
		*
		**************************************************************/
		if (strcmp("--chromosome", argv[i]) == 0 || strcmp("-c", argv[i]) == 0 ) {
		    i++;
		    if ( i >= argc ) {
			std::cout << "Chromosome flag entered but no parameter entered.  Assuming first column contains chromosome/reference id information." << std::endl << std::endl;
			std::cerr << "WARNING: Chromosome flag entered but no parameter entered.  Assuming first column contains chromosome/reference id information." << std::endl << std::endl;
			// return since i is now past the final argument
			return true;
		    } else if ( strncmp(argv[i], "-", 1) == 0 ) {
			std::cout << "Chromosome flag entered but no parameter entered.  Assuming first column contains chromosome/reference id information." << std::endl << std::endl;
			std::cerr << "WARNING: Chromosome flag entered but no parameter entered.  Assuming first column contains chromosome/reference id information." << std::endl << std::endl;
		    } else if ( atoi(argv[i]) && atoi(argv[i]) > 0 ) {
		      // Valid integer and above 0 (rounded by truncating any decimals if float entered).
		      bool _setcolumn = setChromosomeColumn( atoi(argv[i]) );
		      if (!_setcolumn) {
			  std::cerr << "ERROR: Problem setting chromosome column number.  Aborting run." << std::endl << std::endl;
			  return false;
		      }
		    } else if ( !atoi(argv[i]) ) {
		      // Invalid; no number entered.
		      std::cerr << "ERROR: Chromosome flag entered with parameter, but parameter is not a valid column number.  Please enter a valid column number in integer form (the first column is considered 1, not 0).  Aborting run." << std::endl << std::endl;
		      return false;
		    } else if ( atoi(argv[i]) < 0 ) {
		      // Invalid; number is negative
		      std::cerr << "ERROR: Chromosome flag entered with parameter, but parameter is negative number.  Please enter a valid column number in integer form (the first column is considered 1, not 0).  Aborting run." << std::endl << std::endl;
		      return false;
		    }
		}

		/**************************************************************
		*
		* Check for --start / -s arguments
		* Default is column 2 if this flag isn't set.
		*
		**************************************************************/
		if (strcmp("--start", argv[i]) == 0 || strcmp("-s", argv[i]) == 0 ) {
		    i++;
		    if ( i >= argc ) {
			std::cout << "Start flag entered but no parameter entered.  Assuming second column contains start position information." << std::endl << std::endl;
			std::cerr << "WARNING: Start flag entered but no parameter entered.  Assuming second column contains start position information." << std::endl << std::endl;
			// return since i is now past the final argument
			return true;
		    } else if ( strncmp(argv[i], "-", 1) == 0 ) {
			std::cout << "Start flag entered but no parameter entered.  Assuming second column contains start position information." << std::endl << std::endl;
			std::cerr << "WARNING: Start flag entered but no parameter entered.  Assuming second column contains start position information." << std::endl << std::endl;
		    } else if ( atoi(argv[i]) && atoi(argv[i]) > 0 ) {
		      // Valid integer and above 0 (rounded by truncating any decimals if float entered).
		      bool _setcolumn = setStartColumn( atoi(argv[i]) );
		      if (!_setcolumn) {
			  std::cerr << "ERROR: Problem setting start column number.  Aborting run." << std::endl << std::endl;
			  return false;
		      }
		    } else if ( !atoi(argv[i]) ) {
		      // Invalid; no number entered.
		      std::cerr << "ERROR: Start flag entered with parameter, but parameter is not a valid column number.  Please enter a valid column number in integer form (the first column is considered 1, not 0).  Aborting run." << std::endl << std::endl;
		      return false;
		    } else if ( atoi(argv[i]) < 0 ) {
		      // Invalid; number is negative
		      std::cerr << "ERROR: Start flag entered with parameter, but parameter is negative number.  Please enter a valid column number in integer form (the first column is considered 1, not 0).  Aborting run." << std::endl << std::endl;
		      return false;
		    }
		}

		/**************************************************************
		*
		* Check for --end / -e arguments
		* Default is same as start column if this flag isn't set.
		*
		**************************************************************/
		if (strcmp("--end", argv[i]) == 0 || strcmp("-e", argv[i]) == 0 ) {
		    i++;
		    if ( i >= argc ) {
			std::cout << "End flag entered but no parameter entered.  Assuming end column contains is the same column as start (single position annotations)." << std::endl << std::endl;
			std::cerr << "WARNING: End flag entered but no parameter entered.  Assuming end column contains is the same column as start (single position annotations)." << std::endl << std::endl;
			// return since i is now past the final argument
			return true;
		    } else if ( strncmp(argv[i], "-", 1) == 0 ) {
			std::cout << "End flag entered but no parameter entered.  Assuming end column contains is the same column as start (single position annotations)." << std::endl << std::endl;
			std::cerr << "WARNING: End flag entered but no parameter entered.  Assuming end column contains is the same column as start (single position annotations)." << std::endl << std::endl;

		    } else if ( atoi(argv[i]) && atoi(argv[i]) > 0 ) {
		      // Valid integer and above 0 (rounded by truncating any decimals if float entered).
		      bool _setcolumn = setEndColumn( atoi(argv[i]) );
		      if (!_setcolumn) {
			  std::cerr << "ERROR: Problem setting end column number.  Aborting run." << std::endl << std::endl;
			  return false;
		      }
		    } else if ( !atoi(argv[i]) ) {
		      // Invalid; no number entered.
		      std::cerr << "ERROR: End flag entered with parameter, but parameter is not a valid column number.  Please enter a valid column number in integer form (the first column is considered 1, not 0).  Aborting run." << std::endl << std::endl;
		      return false;
		    } else if ( atoi(argv[i]) < 0 ) {
		      // Invalid; number is negative
		      std::cerr << "ERROR: End flag entered with parameter, but parameter is negative number.  Please enter a valid column number in integer form (the first column is considered 1, not 0).  Aborting run." << std::endl << std::endl;		      return false;
		    }
		}

		/**************************************************************
		*
		* Check for --file_type / -f arguments
		* Default is csv.
		*
		**************************************************************/
		if (strcmp("--file_type", argv[i]) == 0 || strcmp("-f", argv[i]) == 0 ) {
		    i++;
		    if ( i >= argc ) {
			std::cout << "File type flag entered but no parameter entered.  Assuming default of CSV file type." << std::endl << std::endl;
			std::cerr << "WARNING: File type flag entered but no parameter entered.  Assuming default of CSV file type." << std::endl << std::endl;
			// return since i is now past the final argument
			return true;
		    } else if ( strncmp(argv[i], "-", 1) == 0 ) {
			std::cout << "File type flag entered but no parameter entered.  Assuming default of CSV file type." << std::endl << std::endl;
			std::cerr << "WARNING: File type flag entered but no parameter entered.  Assuming default of CSV file type." << std::endl << std::endl;
		    } else {
			std::string _tmpFileType(argv[i]);
			std::transform(_tmpFileType.begin(), _tmpFileType.end(), _tmpFileType.begin(), ::tolower);
		      if ( _tmpFileType.compare("csv") == 0 ) {
			// This is the default, but set anyway
			setFileType("csv");
		      } else if ( _tmpFileType.compare("tsv") == 0 ) {
			setFileType("tsv");
		      } else {
			// neither csv or tsv, not supported; Aborting
			std::cout << "File type can only be tsv or csv. " << argv[i] << " was entered and is not a valid file type.  Aborting run." << std::endl << std::endl;
			std::cerr << "ERROR: File type can only be tsv or csv. " << argv[i] << " was entered and is not a valid file type.  Aborting run." << std::endl << std::endl;
		      }

		    }
		}

		/**************************************************************
		*
		* Check for --no_header / -h arguments
		* Default is that annotation files have headers.
		* Note: this is a flag for ALL files; all must either have headers or no headers
		* If you set flag, you are specifying that the files do not have headers
		*
		**************************************************************/
		if (strcmp("--no_header", argv[i]) == 0 || strcmp("-h", argv[i]) == 0 ) {
			setHeader(false);
		}
		/**************************************************************
		*
		* Check for --zero_based / -z arguments
		* Default is that the annotation is 1 based.
		* Note: this is a flag for ALL files; all must either be 0 based or 1 based
		* Setting this flag specifies that the files are 0 based
		*
		**************************************************************/
		if (strcmp("--zero_based", argv[i]) == 0 || strcmp("-z", argv[i]) == 0 ) {
		    setZeroBased(true);
		}

		/**************************************************************
		*
		* Check for --collapse 
		* Default is that the  program does not do collapse annotations.
		* Setting this flag specifies that the program should collapse
		* file coordinates into an output file that contains all continuous
		* coordinate values
		*
		**************************************************************/
		if (strcmp("--collapse", argv[i]) == 0 ) {
		    setCollapse(true);
		}
		/**************************************************************
		*
		* Check for --cpus / -t arguments
		* Default is e 1 thread if this flag isn't set.
		*
		**************************************************************/
		if (strcmp("--cpus", argv[i]) == 0 || strcmp("-t", argv[i]) == 0 || strcmp("--cpu", argv[i]) == 0 ) {
		    i++;
		    if ( i >= argc ) {
			std::cout << "CPUS flag entered but no parameter entered.  Assuming single thread should be used." << std::endl << std::endl;
			std::cerr << "WARNING: CPUS flag entered but no parameter entered.  Assuming single thread should be used." << std::endl << std::endl;
			// return since i is now past the final argument
			return true;
		    } else if ( strncmp(argv[i], "-", 1) == 0 ) {
			std::cout << "CPUS flag entered but no parameter entered.  Assuming single thread should be used." << std::endl << std::endl;
			std::cerr << "WARNING: CPUS flag entered but no parameter entered.  Assuming single thread should be used." << std::endl << std::endl;

		    } else if ( atoi(argv[i]) && atoi(argv[i]) > 0 ) {
		      // Valid integer and above 0 (rounded by truncating any decimals if float entered).
		      bool _setThreads = setThreads( atoi(argv[i]) );
		      if (!_setThreads) {
			  std::cerr << "ERROR: Problem setting thread number.  Aborting run." << std::endl << std::endl;
			  return false;
		      }
		    } else if ( !atoi(argv[i]) ) {
		      // Invalid; no number entered.
		      std::cerr << "ERROR: CPUS flag entered with parameter, but parameter is not a valid thread number.  Please enter a valid integer.  Aborting run." << std::endl << std::endl;
		      return false;
		    } else if ( atoi(argv[i]) < 0 ) {
		      // Invalid; number is negative
		      std::cerr << "ERROR: CPUS flag entered with parameter, but parameter is a negative number.  Please enter a valid integer.  Aborting run." << std::endl << std::endl;		      
		      return false;
		    }
		}
		
		/**************************************************************
		*
		* Check for --snps / -p arguments
		* Default is that the program is agnostic about SNP/SNV status.
		*
		**************************************************************/
		if (strcmp("--snps", argv[i]) == 0 || strcmp("-p", argv[i]) == 0 ) {
		    i++;
		    if ( i >= argc ) {
			std::cout << "SNPS flag entered but no parameter entered.  Disregarding SNPS flag." << std::endl << std::endl;
			std::cerr << "WARNING: SNPS flag entered but no parameter entered.  Disregarding SNPS flag." << std::endl << std::endl;
			// return since i is now past the final argument
			return true;
		    } else if ( strncmp(argv[i], "-", 1) == 0 ) {
			std::cout << "SNPS flag entered but no parameter entered.  Disregarding SNPS flag." << std::endl << std::endl;
			std::cerr << "WARNING: SNPS flag entered but no parameter entered.  Disregarding SNPS flag." << std::endl << std::endl;

		    } else if ( atoi(argv[i]) && atoi(argv[i]) > 0 ) {
		      // Valid integer and above 0 (rounded by truncating any decimals if float entered).
		      bool _setNucleotideColumn = setNucleotideColumn( atoi(argv[i]) );
		      if (!_setNucleotideColumn) {
			  std::cerr << "ERROR: Problem setting nucleotide column.  Aborting run." << std::endl << std::endl;
			  return false;
		      }
		    } else if ( !atoi(argv[i]) ) {
		      // Invalid; no number entered.
		      std::cerr << "ERROR: SNPS flag entered with parameter, but parameter is not a valid number.  Please enter a valid integer.  Aborting run." << std::endl << std::endl;
		      return false;
		    } else if ( atoi(argv[i]) < 0 ) {
		      // Invalid; number is negative
		      std::cerr << "ERROR: SNPS flag entered with parameter, but parameter is a negative number.  Please enter a valid integer.  Aborting run." << std::endl << std::endl;		      
		      return false;
		    }
		}
	}

	if ( annotationFiles.size() < 2 && mappingStyle != COLLAPSE ) {
	    std::cerr << "ERROR: Fewer than two files entered.  Please enter at least two files to crossmap."  << std::endl << std::endl;
	    return false;
	}

  return properArguments;
}





/*****************************************************************************************
 * Cleanup
 ****************************************************************************************/
bool BioMapper::argumentCleanup () {
    bool vec = verifyEndColumn();
    bool slc = setLastColumn();
    annotationFileNumber = returnNumberOfAnnotationFiles ();

    if (vec && slc) {
        return true;
    }
    else {
        return false;
    }
}

bool BioMapper::verifyEndColumn() {
  if (endColumn == -1) {
    std::cout << "Single Position Annotations detected; setting parameters accordingly." << std::endl << std::endl;
    endColumn = startColumn;
  }

  return true;
}

bool BioMapper::setLastColumn() {
  if ( lastColumn < chromosomeColumn )
    lastColumn = chromosomeColumn;
  if ( lastColumn < startColumn )
    lastColumn = startColumn;
  if ( lastColumn < endColumn )
    lastColumn = endColumn;

  if ( lastColumn < 1 ) {
    std::cerr << "ERROR: Error setting last column.  All columns are set to less than one.  Aborting." << endl << endl;
    return false;
  }

  return true;
}







/*****************************************************************************************
 * Setters
 ****************************************************************************************/

bool BioMapper::setChromosomeColumn (int column) {
    chromosomeColumn = column;
    return true;
}
bool BioMapper::setStartColumn (int column) {
    startColumn = column;
    return true;
}
bool BioMapper::setEndColumn (int column) {
    endColumn = column;
    return true;
}

bool BioMapper::setFileName (const char * file_name) {
	outputFileName.clear();
	outputFileName = string(file_name);
	return true;
}

bool BioMapper::setFileType (const std::string file_type) {
  fileType.assign(file_type);
  return true;
}

bool BioMapper::setHeader (bool hdr) {
  header = hdr;
  return true;
}

bool BioMapper::setZeroBased (bool zb) {
  zeroBased = zb;
  return true;
}

int BioMapper::returnNumberOfAnnotationFiles () {
    return annotationFiles.size();
}

bool BioMapper::setThreads (unsigned int _thrds) {
  if (_thrds <= 0) {
      // Shouldn't get here
    return false;
  }
  if (_thrds <= maximum_threads) {
    threads_to_use = _thrds;
  } else {
    threads_to_use = maximum_threads;
  }
    return true;
}

bool BioMapper::setNucleotideColumn (int _nt) {
  if (_nt <=0) {
      // Shouldn't get here
    return false;
  }
  nucleotides = true;
  nucleotideColumn = _nt;
  return true;
}


bool BioMapper::setCollapse(bool _sc) {
  collapse = _sc;
  return true;
}


/****************************************
 * DEBUG
 ****************************************/

void BioMapper::printClassVariables () {
    std::cout << endl << endl << "Variables" << endl;
    std::cout << "=========" << endl;
    std::cout << "chromosomeColumn: " << chromosomeColumn << endl;
    std::cout << "startColumn: " << startColumn << endl;
    std::cout << "endColumn: " << endColumn << endl;
    std::cout << "fileType: " << fileType << endl;
    std::cout << "mappingStyle: " << mappingStyle << endl;
    std::cout << endl << endl;
}

void BioMapper::printFiles () {
    std::cout << "\n\nANNOTATION FILES\n";
    std::cout << "================\n\n";
    int i = 1;
    for (std::string annot : annotationFiles) {
	std::cout << "#" << i << ": " << annot << "\n";
	i++;
    }
}
