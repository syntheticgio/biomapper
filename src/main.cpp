#include "biomapper.hpp"
#include <string>
#include <iostream>

using namespace std;

int main (int argc, char* argv[])
{

	BioMapper myMap;
	bool _tmp = myMap.determineArguments(argc, argv);
	cout << "Determining Arguments was successful: " << _tmp << "\n\n";
	myMap.printFiles();
	
	
}
