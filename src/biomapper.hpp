#include <vector>
#include <string>

#ifndef __BIOMAPPER_HPP_INCLUDED__
#define __BIOMAPPER_HPP_INCLUDED__

class BioMapper
{

	public:
		std::vector <std::string> references;
		BioMapper();

		void determineReferences();
		bool determineArguments(int argc, char** argv);
		void printFiles ();
	
	private:
		std::vector <std::string> annotationFiles;

};

#endif
