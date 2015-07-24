#include "biomapper.hpp"
#include <string>
#include <iostream>

using namespace std;

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
	
	// Verify end column number and set if needed (since this may or may not be passed in as an argument)
	// Also set the last column value to speed up reference id reading.
	bool argumentCleanup = myMap.argumentCleanup();
	
	if (!argumentCleanup) {
	 cout << "Invalid arguments cleanup. See error file for more information." << endl << endl;
	 cerr << "ERROR: Invalid arguments cleanup." << endl << endl;
	 return 0;
	}
	
	#ifdef DEBUG
		myMap.printFiles();
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
	#endif
	
	
}
