#include "biomapper.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

void _debugThreading ();

void mapFiles (BioMapper& myMap);
void returnReferenceIDFromFileName(string & ref);

// Create mutex
std::mutex m;


int main (int argc, char* argv[])
{
	BioMapper myMap;
	bool validateArguments = myMap.determineArguments(argc, argv);
	if (!validateArguments) {
	    std::cout << "Invalid arguments used. See error file for more information." << std::endl << std::endl;
	    std::cerr << "ERROR: Invalid Arguments used." << std::endl << std::endl;
	    return 0;
	}

	// Determine the references
	// TODO: This, validateArguments, and argumentCleanup should be in a single function.
	bool _detRefs = myMap.determineReferences();
	if (!_detRefs) {
		std::cout << "DetermineReferences() failed" << std::endl;
	}

	// Verify end column number and set if needed (since this may or may not be passed in as an argument)
	// Also set the last column value to speed up reference id reading.
	bool argumentCleanup = myMap.argumentCleanup();

	if (!argumentCleanup) {
	 std::cout << "Invalid arguments cleanup. See error file for more information." << std::endl << std::endl;
	 std::cerr << "ERROR: Invalid arguments cleanup." << std::endl << std::endl;
	 return 0;
	}

	#ifdef DEBUG
		myMap.printClassVariables();
	#endif

	// Currently have files from command line
	// also have start, end, and reference (chromosome) id columns.
	// Need to determine references and make sure they match
	// Include 'smart' reference matching flag?

	#ifdef DEBUG
		int numberOfFiles = myMap.returnNumberOfAnnotationFiles();
		std::cout << "Number of Files: " << numberOfFiles << std::endl;
		std::cout << "Number of references: " << myMap.referenceIDs.size() << std::endl;
	#endif

	/*
	#ifdef DEBUG
		std::cout << std::endl << "Verify Threading works:";
		std::cout << std::endl << "=======================" << std::endl;
		std::thread t(_debugThreading);
		t.join();
	#endif
	*/


	#ifdef DEBUG
	  std::cout << "hardware_condurrency() = " << std::thread::hardware_concurrency() << std::endl;
	  std::cout << "max_threads = " << myMap.maximum_threads << std::endl;
	  std::cout << "number of threads to use (--cpus): " << myMap.threads_to_use << std::endl;
	#endif

	std::vector <std::thread> allThreads;

	for (unsigned int i = 0; i < myMap.threads_to_use; i++) {
	    allThreads.push_back( std::thread(mapFiles, std::ref(myMap)) );
	}

	std::cout << "Wait for threads to join" << std::endl << std::endl;

	// Wait for all threads to finish
	for (auto& _individualThread: allThreads) _individualThread.join();

	std::cout << "All threads have finished" << std::endl << std::endl;

	// Open Output file
	ofstream outputFile(myMap.outputFileName);
	// write a header
	outputFile << "chromosome,start,end" << endl;

	// Need to now generate mapped output file.
	
	for (size_t i = 0; i < myMap.dataFiles.size(); i++) {
		// myMap.dataFiles should be each of the data files
		ifstream crossingFile;
		string file = myMap.dataFiles[i];
		crossingFile.open(myMap.dataFiles[i], ios::binary | ios::ate | ios::in);
		string ref = myMap.dataFiles[i];
		
		returnReferenceIDFromFileName(ref);
		
		if(crossingFile.is_open()){			  
			// TODO: Need to check to make sure size of the file neatly fits into 32 bit chunks or adjust
			// accordingly.
			//long int size = crossingFile.tellg(); // Returns the size of the file.
			//long int sizeOfFileInInts= size/4;

			crossingFile.seekg (0, ios::beg); // return pointer to begining of file
			//char * memblock = new char [size];
			
			unsigned int _buffer = 0; // Buffer recording the binary data
			long long int counter = 0; // Counter to determine which iteration we are at for data extraction

			vector <unsigned long long int> _start;
			vector <unsigned long long int> _end;

			// TODO: This might be assuming I'm guarenteeing an whole number of integers are written (currently I believe I am)


			while(crossingFile.read((char *)&_buffer, sizeof(_buffer))) {
				//Data is read into _buffer.
			   if(_buffer > 0) {
					//important data found.
					if (~_buffer == 0) {
						// Range full currently
						counter++;
						continue;
					} 
					// Need to find where 1s start or where they end.
					// How to determine first 1?

					// TODO: This method is ignoring if there are small gaps and collapsing!!  Need to check individual bits instead of
					// relying on __builtins

					int leftBit = __builtin_clz(_buffer); // Return the position of the highest bit
					
					if (leftBit > 0) {
						// There are leading 0 bits
						unsigned long long int _tmp = counter * 32 + leftBit + 1;	
						// Set the start position of this range.
						_start.push_back(_tmp);
					} else {
						// If trailing 0s, set end position
						int rightBit = __builtin_ffs(_buffer); // This returns the index + 1 of the right most bit											
						if(rightBit > 0) {
							unsigned long long int _tmp = counter * 32 + (32 - rightBit + 1);
							_end.push_back(_tmp);
						}	
					}	
					
			   }
			   counter++; // Increment to know the future offset
			}					


			// Print out the ranges here
			//TODO: Need to adjust output here to also display annotations ingested
			for (size_t j =0; j < _start.size(); j++) {
				cout <<"REF:" << ref << "\tSTART: " << _start[j] << "\tEND: " << _end[j] << endl;
				outputFile << ref << "," << _start[j] << "," << _end[j] << endl;
			}			
			
			// Close the file
			crossingFile.close();
						
			//TODO: This could be set as a parameter whether or not to keep intermediate files
			// Delete the temorary data file
			std::remove(file.c_str());
			
		} else {
			cout << "Failed to open the file: " << myMap.dataFiles[i] << "." << endl;
		}
	}
	
	// Close the write file
	outputFile.close();

}


void mapFiles (BioMapper& myMap) {
		std::string refID;

		{
			std::lock_guard<std::mutex> lock(m);
			if (myMap.threads.empty()) {
			  #ifdef DEBUG
			std::cout << "No more threads, exiting function." << std::endl;
			  #endif
			  return;
			}
			else {
			  refID=myMap.threads.back();
			  myMap.threads.pop_back();
			}
		}

        #ifdef DEBUG
                std::cout << "In mapFiles function" << std::endl;
        #endif
        // Define reference for local variable, not strictly necessary
        // but help avoid changing variable inadvertantly since passed
        // in by reference.
        std::string _refID = refID;

        // Check to see if the reference exists in the map generated earlier
        // If reference is not in the reference set, then there is
        if ( myMap.referenceIDs[_refID] != myMap.annotationFileNumber ) {
                #ifdef DEBUG
                        std::cout << "Reference " << _refID << " not found in all files" << std::endl;
                #endif
                return ;
        }

        // Declare two vectors of bits (int64_t)
        // Each position in the reference id will be a single bit
        // If the bit is turned on, there was a match with all files so far
        // If the bit is tunred off, at least one file had no data at that position
        // The content of the data doesn't matter (i.e. the annotations
        // do not need to be the same, only exist at the same position).
        // Partial overlaps are allowed

        vector <int> basemap;
        vector <int> tmpmap;

		vector <int> ntmap;
		vector <int> tmpntmap;
	
        // Loop through each annotation file and compare position mapping culmatively
        for (int i = 0; i < myMap.annotationFileNumber; i++) {
                //
                // Set proper map (base or tmp map)
                // If it is the first time through the loop we make a pointer to
                // the main mapping vector - basemap.
                // If it any later iteration, we record the position bits for that
                // file in the temporary bitmap.  These two will be collapsed at the
                // end of the iteration.  This allows us to only carry through from
                // the first two iterations any positions that we know are of interest
                // and ignore those positions that by definition cannot satisfy the
                // mapping criteria.

                vector <int>& bm = (i == 0) ? basemap : tmpmap;
				//vector <int>& ntbm = (i == 0) ? ntmap : tmpntmap;

		
                // Open current annotatoin file
                // std::ifstream::in tag redundant; but just incase ifstream changes in the future include it
                ifstream annot;
                annot.open(myMap.annotationFiles[i], std::ifstream::in);

                std::string row;

                // Remove the header if it exists
                // Record it in vector/map for output
                if (myMap.header) {
                    getline(annot, row);
                    myMap.headerRows.push_back(row);
                }

                // Determine the splitting character.  By default it is a comma (,) but
                // it can be changed here to a tab.  Other filetypes could be added here
                // as long as theya re supported in the argument interpreting function
                char splitter = ',';
                if ( myMap.fileType.compare("tsv") == 0 ) {
                        splitter = '\t';
                }

                // assuming the file is not sorted, if sorted use different algorithm and only
                // search only parts of the file that match the reference
                // TODO: If file is sorted, can do a binary search if we have the ranges
                // of where the refIDs start and end in the file.

                while ( getline(annot, row) ) {
                        stringstream _rowElements(row);
                        string _element, _ref;
                        int ii = 1;
                        long _start = -1;
                        long _end = -1;
                        bool validMatch = false;

                        // Loop through each element in the row
                        while ( getline(_rowElements, _element, splitter) ) {

                                // Check to see if we've gotten to the chromosome column
                                if ( ii == myMap.chromosomeColumn ) {
                                        // Check to make sure that the reference sequence is the correct one
                                        // or skip to the next row if not.
                                        if ( _element.compare(_refID) == 0 ) {
                                                _ref = _element;
                                                validMatch = true;
                                        } else {
                                                validMatch = false;
                                                break;
                                        }
                                } else if (ii == myMap.startColumn) {
                                        // Check for the start column value
                                        // TODO: right now we assume it is a valid number or it returns a 0
                                        // Should also check to see if the errorEnd returns to a valid string
                                        char * errorEnd;
                                        _start = strtol(_element.c_str(), &errorEnd, 10);
                                        if ( _start < 0 ) {
                                                // position is negative, invalid so skip
                                                validMatch = false;
                                                break;
                                        }
                                }
                                else if (ii == myMap.endColumn) {
                                        // Check for the end column value
                                        // TODO: right now we assume it is a valid number or it returns a 0
                                        // Should also check to see if the errorEnd returns to a valid string
                                        char * errorEnd;
                                        _end = strtol(_element.c_str(), &errorEnd, 10);
                                        if ( _end < 0 ) {
                                                // position negative
                                                validMatch = false;
                                                break;
                                        }
                                }
                                if (ii >= myMap.chromosomeColumn && ii >= myMap.startColumn && ii >= myMap.endColumn) {
                                        break;
                                }
                                ii++;
                        }

                        if (!validMatch) continue;

                        // Have row details here, set bits
                        // Need to make sure map size is appropriate.
                        int bucket1 = _start / 32;
                        int bucket2 = _end / 32;
                        long trueStart = -1;
                        long trueEnd = -1;

                        // If end and start are reversed (end is less than the start value)
                        // correct to fix the data or user error.
                        // Initialize tmpmap vector with the size necessary (positions / 64)
                        // and set all bits off (0)
                        if (_end >= _start)
                        {
                                // This is the case where start is prior or equal to end
                                trueStart = _start;
                                trueEnd = _end;
								if (bm.size() < (bucket2 + 1)){
								  bm.resize(bucket2 + 1, 0);
								}
                        } else {
                                // This is the case where start is after end (columns are mixed/reverse strand, etc)
                                trueStart = _end;
                                trueEnd = _start;
								if (bm.size() < (bucket1 + 1)){
								  bm.resize(bucket1 + 1, 0);
								}
                        }

                        // Correct if 1 based instead of 0 based
                        if (!myMap.zeroBased) {
                                trueStart--;
                                trueEnd--;
                        }

                        // Map is now properly sized to handle this range

                        for (long jj = trueStart; jj <= trueEnd; jj++) {
                                // Set the bucket that contains the position of interest
                                int _bucket = jj / 32;
                                // Get the bit within the bucket for the position of interest
                                int _pos = 31 - (jj % 32);
								//int _pos = jj % 32;
								
                                // Set positional bit
                                bm[_bucket] = bm[_bucket] | ( 1 << _pos );
                        }
                }
                // Record bits in the main bitmap
                for (unsigned int j = 0; j < bm.size(); j++) {
		    		if (myMap.mappingStyle == OVERLAP) {
            			basemap[j] = basemap[j] & bm[j];
				    } else if (myMap.mappingStyle == EXCLUSIVE) {
						// This XOR doesn't quite do what we want for this.  For now it is accurate; but it should really
						// be able to distinguish between the different maps.
				        basemap[j] = basemap[j] ^ bm[j];
				    } else if (myMap.mappingStyle == COLLAPSE) {
					  
						basemap[j] = basemap[j] | bm[j];
					} else {
						std::cout << "Error in recording information to bitmap.  No propper Mapping Style set" << endl;
					}
                }

                annot.close();
        }

        #ifdef DEBUG
                std::cout << "REF ID: " << _refID << std::endl;
                for (unsigned int cnt = 0; cnt < basemap.size(); cnt++) {
                        std::bitset<32> x(basemap[cnt]);
                        //std::cout << (cnt*32+1) << '\t' << x << '\t'<< ((cnt+1)*32) << std::endl;
                }
                //std::cout << std::endl << std::endl;
        #endif

        std::ofstream refIDOutputFile;
        std::string __refID = "__tmp__" + refID;

    // Re-lock the data and write
    // to the data file so we know
    // which files to read later.
    {
        std::lock_guard<std::mutex> lock(m);
        myMap.dataFiles.push_back(__refID);

    }

	#ifdef DEBUG
	   std::cout << "Writing to file: " << __refID << std::endl;
	   std::cout << "basemap size: " << basemap.size() << std::endl;
	#endif
    refIDOutputFile.open(__refID, ios::binary);

	// Save the data in binary within the temproary dat files
    for (unsigned int i = 0; i < basemap.size(); i++) {
	  cout << "basemap[i]: " << basemap[i] << " i: " << i << endl;
      refIDOutputFile.write(reinterpret_cast<const char*>(&basemap[i]), sizeof basemap[i]);
    }

    refIDOutputFile.close();

    #ifdef DEBUG
        std::cout << "Finished thread, launching new one if needed." << std::endl;
    #endif

    // Recursive; keep calling self until all threads are exhausted
    mapFiles(myMap);

  return ;
}

void returnReferenceIDFromFileName(string & ref) {
	string _tmp = "__tmp__";
	string::size_type i = ref.find(_tmp);
	if (i != string::npos)
	  ref.erase(i, _tmp.length());
}

void _debugThreading () {
	cout << "Hello World!" << endl;
}
