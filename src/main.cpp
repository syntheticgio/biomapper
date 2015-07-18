#include "biomapper.hpp"
#include <string>
#include <iostream>

using namespace std;

int main (int argc, char* argv[])
{

	BioMapper myMap;
	bool validateArguments = myMap.determineArguments(argc, argv);
	if (!validateArguments) {
	    cout << "Invalid arguments used \n\n";
	    cerr << "Invalid Arguments\n\n";
	}
	myMap.printFiles();
	
	// Currently have files from command line
	// Need to determine references and make sure they match
	// Include 'smart' reference matching flag?
	
	int numberOfFiles = myMap.returnNumberOfAnnotationFiles();
	cout << "Number of Files: " << numberOfFiles << endl;
	
	
}
