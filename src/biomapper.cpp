#include "biomapper.hpp"
#include <iostream>
#include <fstream>
#include <string.h>


//using namespace std;

BioMapper::BioMapper () : mappingStyle{OVERLAP} { }

void BioMapper::determineReferences() {
    std::ifstream annot1;
    std::ifstream annot2;
  
    annot1.open(annotationFiles[0]);
    annot2.open(annotationFiles[1]);
  
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
			    std::cerr << "No annotation files following the --annotations/-a flag\n\n";
			    return false;
			}
			while (strncmp(argv[i], "-", 1) != 0) {
				annotationFiles.push_back(argv[i]);
				i++;
				if (i >= argc) {
					i--;
					break;
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
			std::cerr << "No annotation files following the --map_type/-m flag; annotation flag should not be at the end of the line.\n\n";
			return false;
		    } else if ( strncmp(argv[i], "-", 1) == 0 ) {
			std::cerr << "No annotation files following the --map_type/-m flag.\n\n";
			return false;
		    }
		    if ( strcmp("overlap", argv[i]) == 0 ) {
			mappingStyle = OVERLAP;
		    } else if ( strcmp("exclusive", argv[i]) == 0) {
			mappingStyle = EXCLUSIVE;
		    } else {
			std::cerr << "Improper type for --map_type/-m.  You must use either 'overlap' or 'exclusive' as the flag option.\n\n";
			return false;
		    }
		}
	}
return properArguments;
}

int BioMapper::returnNumberOfAnnotationFiles () {
    return annotationFiles.size();
}
