#include "biomapper.hpp"
#include <iostream>
#include <fstream>
#include <string.h>


//using namespace std;

BioMapper::BioMapper () : mappingStyle(OVERLAP), chromosomeColumn(1), startColumn(2), endColumn(-1), fileType("csv"), header(true), lastColumn(-1) { }

bool BioMapper::determineReferences() {

  std::cout << "Determining references for crossmapping..." << endl << endl;
  
  
  for (std::string annotFile : annotationFiles ) {
    std::ifstream annot;
    annot.open(annotFile);
    // Read in all references as a dictionary
    
    string annotRow;
    std::map <std::string, bool> _refIDs;
    
    if (header) {
      // remove first line and save for future use
      std::string _row;
      std::getline(annot, row);
      if (!row.size()) {
	std::cerr << "ERROR: No file size for " << annotFile << ".  Aborting." << endl << endl;
	return false;
      }
      headerRows.push_back(row);
    }
    
    while ( std::getline(annot, row) ) {
	std::stringstream _rowElements(row);
	std::string _element;
	int i = 1;
	char splitter = ',';
	if ( fileType.compare("tsv") == 0 ) {
	    splitter = '\t';
	}
	while ( std::getline(_rowElements, _element, splitter) ) {
	    if ( i == chromosomeColumn ) {
	      std::map <std::string, bool>::iterator it;
	      it = _refIDs.find(_element);
	      if ( it == _refIDs.end() ) {
		// new element for this file
		int _r = referenceIDs[_element];
		// Should be 0 if this is the first _element added to referenceIDs
		_r++;
		referenceIDs[_element] = _r;
	      }
	      // If already updated referenceIDs for this _element, ignore and move to next row
	      
	      // Break out of loop and move to next row
	      break;
	    } 
	}
    }
    
    
  }
  
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
			
			if ( annotationFiles.size() < 2 ) {
			    std::cerr << "ERROR: Fewer than two files entered.  Please enter at least two files to crossmap."  << std::endl << std::endl;
			    return false;
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
			std::string _tmpMapingType(argv[i]);
			std::transform(_tmpMappingType.begin(), _tmpFileType.end(), ::tolower);
			if ( _tmpMappingType.compare("overlap") == 0 ) {
			    mappingStyle = OVERLAP;
			    std::cout << "Mapping Style: Overlap (output positions that exist in all entered annotation files." << std::endl;
			} else if ( _tmpMapingType.compare("exclusive") == 0) {
			    mappingStyle = EXCLUSIVE;
			} else {
			    std::cerr << "ERROR: Improper type for --map_type/-m.  You must use either 'overlap' or 'exclusive' as the flag option." << std::endl << std::endl;
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
		    } else if ( atoi(argv[i] && atoi(argv[i] > 0 ) {
		      // Valid integer and above 0 (rounded by truncating any decimals if float entered).  
		      bool _setcolumn = setChromosomeColumn( atoi(argv[i]) );
		      if (!_setcolumn) {
			  std::cerr << "ERROR: Problem setting chromosome column number.  Aborting run." << std::endl << std::endl;
			  return false;
		      }
		    } else if ( !atoi[argv[i] ) {
		      // Invalid; no number entered.
		      std::cerr << "ERROR: Chromosome flag entered with parameter, but parameter is not a valid column number.  Please enter a valid column number in integer form (the first column is considered 1, not 0).  Aborting run." << std::endl << std::endl;
		      return false;
		    } else if ( atoi(argv[i] < 0) ) {
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
		    } else if ( atoi(argv[i] && atoi(argv[i] > 0 ) {
		      // Valid integer and above 0 (rounded by truncating any decimals if float entered).  
		      bool _setcolumn = setStartColumn( atoi(argv[i]) );
		      if (!_setcolumn) {
			  std::cerr << "ERROR: Problem setting start column number.  Aborting run." << std::endl << std::endl;
			  return false;
		      }
		    } else if ( !atoi[argv[i] ) {
		      // Invalid; no number entered.
		      std::cerr << "ERROR: Start flag entered with parameter, but parameter is not a valid column number.  Please enter a valid column number in integer form (the first column is considered 1, not 0).  Aborting run." << std::endl << std::endl;
		      return false;
		    } else if ( atoi(argv[i] < 0) ) {
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
			
		    } else if ( atoi(argv[i] && atoi(argv[i] > 0 ) {
		      // Valid integer and above 0 (rounded by truncating any decimals if float entered).  
		      bool _setcolumn = setEndColumn( atoi(argv[i]) );
		      if (!_setcolumn) {
			  std::cerr << "ERROR: Problem setting end column number.  Aborting run." << std::endl << std::endl;
			  return false;
		      }
		    } else if ( !atoi[argv[i] ) {
		      // Invalid; no number entered.
		      std::cerr << "ERROR: End flag entered with parameter, but parameter is not a valid column number.  Please enter a valid column number in integer form (the first column is considered 1, not 0).  Aborting run." << std::endl << std::endl;
		      return false;
		    } else if ( atoi(argv[i] < 0) ) {
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
			std::transform(_tmpFileType.begin(), _tmpFileType.end(), ::tolower);
		      if ( _tmpFileType.compare("csv") == 0 ) {
			// This is the default, but set anyway
			setFileType("csv");
		      } else if ( _tmpFileType.compare("tsv") == 0 ) {
			setFileType("tsv");
		      } else {
			// neither csv or tsv, not supported; Aborting
			std::cout << "File type can only be tsv or csv. " << argv[i] << " was entered and is not a valid file type.  Aborting run." << endl << endl;
			std::cerr << "ERROR: File type can only be tsv or csv. " << argv[i] << " was entered and is not a valid file type.  Aborting run." << endl << endl;
		      }
			
		    }
		}
		
		/**************************************************************
		* 
		* Check for --header / -h arguments
		* Default is that annotation files have headers.
		* Note: this is a flag for ALL files; all must either have headers or no headers
		* Set with true or false
		* 
		**************************************************************/
		if (strcmp("--header", argv[i]) == 0 || strcmp("-h", argv[i]) == 0 ) {
		    i++;
		    if ( i >= argc ) {
			std::cout << "Header flag entered but no parameter entered.  Assuming default of files all having a header." << std::endl << std::endl;
			std::cerr << "WARNING: Header flag entered but no parameter entered.  Assuming default of files all having a header." << std::endl << std::endl; 
			// return since i is now past the final argument
			return true;
		    } else if ( strncmp(argv[i], "-", 1) == 0 ) {
			std::cout << "Header flag entered but no parameter entered.  Assuming default of files all having a header." << std::endl << std::endl;
			std::cerr << "WARNING: Header flag entered but no parameter entered.  Assuming default of files all having a header." << std::endl << std::endl; 
		    } else {
			std::string _tmpHeader(argv[i]);
			std::transform(_tmpHeader.begin(), _tmpFileType.end(), ::tolower);
		      if ( _tmpHeader.compare("true") == 0 ) {
			// This is the default, but set anyway
			setHeader(true);
		      } else if ( _tmpHeader.compare("false") == 0 ) {
			setHeader(false);
		      } else {
			// neither true or false; Aborting
			std::cout << "Header can only be to true or false.  If no header flag is provided, the program will assume true. " << argv[i] << " was entered.  Aborting run." << endl << endl;
			std::cerr << "ERROR: Header can only be to true or false.  If no header flag is provided, the program will assume true. " << argv[i] << " was entered.  Aborting run." << endl << endl;
		      }
			
		    }
		}
		
	}
	
  return properArguments;
}

bool BioMapper::argumentCleanup () {
  bool vec = verifyEndColumn();
  bool slc = setLastColumn();
  
  if (vec && slc)
    return true;
  else
    return false;
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
    lastColumn == chromosomeColumn;
  if ( lastColumn < startColumn )
    lastColumn == startColumn;
  if ( lastColumn < endColumn )
    lastColumn == endColumn;
  
  if ( lastColumn < 1 ) {
    std::cerr << "ERROR: Error setting last column.  All columns are set to less than one.  Aborting." << endl << endl;
    return false;  
  }
  
  return true;
}

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

bool BioMapper::setFileType (const std::string file_type) {
  fileType.assign(file_type);
  return true;
}

bool BioMapper::setHeader (bool hdr) {
  header = hdr;
  return true;
}

/****************************************
 * DEBUG
 ****************************************/

int BioMapper::returnNumberOfAnnotationFiles () {
    return annotationFiles.size();
}

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