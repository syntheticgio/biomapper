#include <vector>
#include <string>

#ifndef __BIOMAPPER_HPP_INCLUDED__
#define __BIOMAPPER_HPP_INCLUDED__

enum CrossMapType { OVERLAP, EXCLUSIVE };

class BioMapper
{

	public:
		
	  BioMapper();

	  void determineReferences();
	  bool determineArguments(int argc, char** argv);

	  // Debugging
	  void printFiles ();
	  int returnNumberOfAnnotationFiles ();
	
	private:
	  CrossMapType mappingStyle;
	  std::vector <std::string> annotationFiles;
	  std::vector <std::string> referenceNames;

};

#endif
