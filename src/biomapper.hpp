#include <vector>
#include <string>
#include <map>
#include <mutex>

#ifndef __BIOMAPPER_HPP_INCLUDED__
#define __BIOMAPPER_HPP_INCLUDED__

//class idx
//{
//public:
//  idx () : val(0) { };
//  int64_t val;
//};


enum CrossMapType { OVERLAP, EXCLUSIVE, COLLAPSE };

class BioMapper
{

	public:
        // Default constructor
        BioMapper();

        std::mutex mtx;

        bool determineReferences(); // Determine the references that are in all files
        bool determineArguments(int argc, char** argv); // Determine the command line arguments

        // Finish up arguments by verifying that the columns
        // meet criteria for each column.
        bool argumentCleanup ();

        // Getters
        int returnNumberOfAnnotationFiles ();

        //
        // Class Public Variables
        //
		
        int chromosomeColumn; // Column number (base 1) where the chromosomes/segments/sequence IDs are recorded
        int startColumn; // Column number (base 1) where the start values are recorded
        int endColumn; // Column number (base 1) where the end values are recorded
        int lastColumn; // Last column out of chromosome, start, and end for quicker reading
		int nucleotideColumn; // The column that is the nucleotide

        int annotationFileNumber; // The number of annotation files that are entered

        unsigned int threads_to_use; // Number of threads to use
        const unsigned int maximum_threads; // Maximum number of threads supported by the system

        bool header; // If files have a header (true)
		bool zeroBased; // Whether the file is 0 (true) based or 1 based (false)

		bool collapse;
		bool nucleotides;

		std::string outputFileName; // The name for the output file for mapped results.
        std::string fileType; // Allowed file types (i.e. csv, tsv, etc.)
        std::vector <std::string> annotationFiles; // List of annotation files
        std::vector <std::string> threads; // vector of threads that are launched
        std::vector <std::string> dataFiles; // List of the output data files (one for each chromosome)
        std::map <std::string, int> referenceIDs; // Dictionary of the reference IDs with the number of files that matched them
        
	// Dictionary of the header rows from each file
        // Position here matches file position in annotationFiles
        //std::map <std::string, std::string> headerRows;
	std::vector <std::string> headerRows;

        CrossMapType mappingStyle; // Enum of different mapping styles possible

        // Debugging
        void printFiles ();
        void printClassVariables ();

	private:
		// Class setters
		bool setChromosomeColumn (int column);
		bool setStartColumn (int column);
		bool setEndColumn (int column);
		bool setFileType (const std::string file_type);
		bool setHeader (bool hdr);
		bool setZeroBased (bool zb);
		bool setThreads (unsigned int _thrds);
		bool setNucleotideColumn(int _nt);
		bool setFileName (const char * file_name);
		bool setCollapse(bool _sc);

        //
        // Cleanup
        //

        // Make sure that the end column is set to same as start column if
        // not passed by user (SNPs)
        bool verifyEndColumn();
        // Record which column is the last one required to read for mapping
        // to speed up the amount of column processing (escape when beyond
        // last column range).
        bool setLastColumn();
};

#endif
