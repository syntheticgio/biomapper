#include <vector>
#include <string>
#include <map>

#ifndef __BIOMAPPER_HPP_INCLUDED__
#define __BIOMAPPER_HPP_INCLUDED__

enum CrossMapType { OVERLAP, EXCLUSIVE };
//class idx
//{
//public:
//  idx () : val(0) { };
//  int64_t val;
//};


class BioMapper
{

	public:
		
	  BioMapper();
	
	  bool determineReferences();
	  bool determineArguments(int argc, char** argv);
	  bool mapFiles(std::string refID);
	  void launchThreads();	

	  bool argumentCleanup ();
	  int returnNumberOfAnnotationFiles ();
		 
 
	  std::map <std::string, int> referenceIDs;

	// Debugging
	  void printFiles ();
	  void printClassVariables ();
	
	private:
	  	// Class Variables
		CrossMapType mappingStyle;
		int chromosomeColumn;
	  	int startColumn;
	  	int endColumn;
	  	int lastColumn;
	  	bool header;
	  	int annotationFileNumber;
	  	std::string fileType;
	  	std::vector <std::string> annotationFiles;
	  	std::vector <std::string> headerRows;
	  	bool zeroBased;
		const int maximum_threads;
	
		// Class setters
		bool setChromosomeColumn (int column);
		bool setStartColumn (int column);
		bool setEndColumn (int column);
		bool setFileType (const std::string file_type);
		bool setHeader (bool hdr);
		bool setZeroBased (bool zb);

	  // Cleanup
	  bool verifyEndColumn();
	  bool setLastColumn();
};



#endif
