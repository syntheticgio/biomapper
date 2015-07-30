#include "biomapper.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <thread>

using namespace std;

// DEBUG
void _debugThreading (string refID);
//

int main (int argc, char* argv[])
{

	#ifdef DEBUG
		cout << endl << endl << "Debug Is Set" << endl << endl;
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
		string ts = "Test";
		std::thread t(_debugThreading, ts);
		t.join();
	#endif

	/*
	for (auto& refIDs : myMap.referenceIDs) {
		#ifdef DEBUG
			cout << refIDs.first << " has " << refIDs.second << " file hits." << endl;
		#endif
		if (refIDs.second == myMap.returnNumberOfAnnotationFiles()) {
			//vector <int> basemap;
			std::thread 
			bool _res = myMap.mapFiles(refIDs.first);
			cout << "The result for " << refIDs.first << " is " << _res << endl;
		}
	}
	*/

	myMap.launchThreads();	
	
	
}


void _debugThreading (string refID) {
	cout << "Hi: " << refID << endl;
}
