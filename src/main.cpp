#include "biomapper.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <thread>

using namespace std;

void _debugThreading ();

bool mapFiles (string refID, BioMapper& myMap);

int main (int argc, char* argv[])
{

#ifdef DEBUG
	cout << "\n\nDebug works\n\n";
#endif

	BioMapper myMap;
	bool validateArguments = myMap.determineArguments(argc, argv);
	if (!validateArguments) {
	    cout << "Invalid arguments used. See error file for more information." << endl << endl;
	    cerr << "ERROR: Invalid Arguments used." << endl << endl;
	    return 0;
	}

	// Determine the references
	// TODO: This, validateArguments, and argumentCleanup should be in a single function.
	bool _detRefs = myMap.determineReferences();
	if (!_detRefs) {
		cout << "DetermineReferences() failed" << endl;
	}
	
	// Verify end column number and set if needed (since this may or may not be passed in as an argument)
	// Also set the last column value to speed up reference id reading.
	bool argumentCleanup = myMap.argumentCleanup();
	
	if (!argumentCleanup) {
	 cout << "Invalid arguments cleanup. See error file for more information." << endl << endl;
	 cerr << "ERROR: Invalid arguments cleanup." << endl << endl;
	 return 0;
	}
	
	#ifdef DEBUG
		//myMap.printFiles();
		myMap.printClassVariables();
	#endif
	
	// Currently have files from command line
	// also have start, end, and reference (chromosome) id columns.
	// Need to determine references and make sure they match
	// Include 'smart' reference matching flag?

	// Loop through
	// mapFiles(&refID)
	//	
	#ifdef DEBUG
		int numberOfFiles = myMap.returnNumberOfAnnotationFiles();
		cout << "Number of Files: " << numberOfFiles << endl;
		cout << "Number of references: " << myMap.referenceIDs.size() << endl;
	#endif

	#ifdef DEBUG
		cout << endl << "Verify Threading works:";
		cout << endl << "=======================" << endl;
		std::thread t(_debugThreading);
		t.join();
	#endif
	
	for (auto& refIDs : myMap.referenceIDs) {
		#ifdef DEBUG
			cout << refIDs.first << " has " << refIDs.second << " file hits." << endl;
		#endif
		if (refIDs.second == myMap.returnNumberOfAnnotationFiles()) {
			//vector <int> basemap;
			bool _res = mapFiles(refIDs.first, myMap);
			cout << "The result for " << refIDs.first << " is " << _res << endl;
		}
	}	
	
	
}

bool mapFiles (string refID, BioMapper& myMap) {

        #ifdef DEBUG
                cout << "In mapFiles function" << endl;
        #endif  
        // Define reference for local variable, not strictly necessary
        // but help avoid changing variable inadvertantly since passed
        // in by reference.
        string _refID = refID;
        
        // Check to see if the reference exists in the map generated earlier
        // If reference is not in the reference set, then there is 
        if ( myMap.referenceIDs[_refID] != myMap.annotationFileNumber ) {
                #ifdef DEBUG
                        cout << "Reference " << _refID << " not found in all files" << endl;
                #endif
                return 0 ;
        }
        
        //
        // Declare two vectors of bits (int64_t)
        // Each position in the reference id will be a single bit
        // If the bit is turned on, there was a match with all files so far
        // If the bit is tunred off, at least one file had no data at that position
        // The content of the data doesn't matter (i.e. the annotations
        // do not need to be the same, only exist at the same position).
        // Partial overlaps are allowed
         
        vector <int> basemap;
        vector <int> tmpmap;

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

                // Open current annotatoin file
                // std::ifstream::in tag redundant; but just incase ifstream changes in the future include it
                ifstream annot;
                annot.open(myMap.annotationFiles[i], std::ifstream::in);

                string row;
                
                // Remove the header if it exists
                //TODO: We will likely want to save the header in the future for 
                // output.
                if (myMap.header) {
                        getline(annot, row);
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
                        int i = 1;
                        long _start = -1;
                        long _end = -1;
                        bool validMatch = false;

                        // Loop through each element in the row
                        while ( getline(_rowElements, _element, splitter) ) {

                                // Check to see if we've gotten to the chromosome column
                                if ( i == myMap.chromosomeColumn ) {
                                        // Check to make sure that the reference sequence is the correct one
                                        // or skip to the next row if not.
                                        if ( _element.compare(_refID) == 0 ) {
                                                _ref = _element;
                                                validMatch = true;
                                        } else {
                                                validMatch = false;
                                                break;
                                        }       
                                } else if (i == myMap.startColumn) {
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
                                else if (i == myMap.endColumn) {
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
                                if (i >= myMap.chromosomeColumn && i >= myMap.startColumn && i >= myMap.endColumn) {
                                        break;
                                }
                                i++;
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
                                bm.resize(bucket2 + 1, 0);
                        } else {
                                // This is the case where start is after end (columns are mixed/reverse strand, etc)
                                trueStart = _end;
                                trueEnd = _start;
                                bm.resize(bucket1 + 1, 0);
                        }

                        // Correct if 1 based instead of 0 based
                        if (!myMap.zeroBased) {
                                trueStart--;
                                trueEnd--;
                        }
        
                        // Map is now properly sized to handle this range
                        
                        for (long ii = trueStart; ii <= trueEnd; ii++) {
                                // Set the bucket that contains the position of interest
                                int _bucket = ii / 32;
                                // Get the bit within the bucket for the position of interest
                                int _pos = 31 - (ii % 32);

                                // Set positional bit   
                                bm[_bucket] = bm[_bucket] | ( 1 << _pos );
                        }
                }
                // Record bits in the main bitmap
                for (unsigned int j = 0; j < bm.size(); j++) {
                         basemap[j] = basemap[j] & bm[j];
                }

                annot.close();
        }

        #ifdef DEBUG
                cout << "REF ID: " << _refID << endl;
                for (unsigned int i = 0; i < basemap.size(); i++) {
                        bitset<32> x(basemap[i]);
                        cout << (i*32+1) << '\t' << x << '\t'<< ((i+1)*32) << endl;
                }
                cout << endl << endl;
                cout << "Now returning the vector back to the main function." << endl << endl;
        #endif
        
        ofstream refIDOutputFile;
        string __refID = "tmp/__tmp__" + refID;
        refIDOutputFile.open(__refID, ios::binary);
        
        for (unsigned int i = 0; i < basemap.size(); i++) {
                refIDOutputFile << basemap[i];
        }

   return true;
}



void _debugThreading () {
	cout << "Hello World!" << endl;
}
