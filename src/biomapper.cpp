#include "biomapper.hpp"
#include <iostream>
#include <string.h>

using namespace std;

BioMapper::BioMapper () { }
 
void BioMapper::determineReferences() {
  
}

void BioMapper::printFiles () {
    cout << "\n\nANNOTATION FILES\n";
    cout << "================\n\n";
    int i = 1;
    for (string annot : annotationFiles) {
        cout << "#" << i << ": " << annot << "\n";
        i++;
    }
}

bool BioMapper::determineArguments(int argc, char** argv) {
	// Determine command line arguments
 	bool properArguments = true;
 	for (int i = 1; i < argc; i++) {
 		if (strcmp("--annotations", argv[i]) == 0 || strcmp("-a", argv[i]) == 0 ) {
			i++;
			while (strncmp(argv[i], "-", 1) != 0) {
				annotationFiles.push_back(argv[i]);
				i++;
				if (i >= argc) {
					i--;
					break;
				}
			}
		}
	}
return properArguments;
}
