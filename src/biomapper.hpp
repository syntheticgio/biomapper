#include <vector>
#include <string>
#include <map>
#include <mutex>

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
        // Default constructor
        BioMapper();

        std::mutex mtx;

        // Determine the references that are in all files
        bool determineReferences();
        // Determine the command line arguments
        bool determineArguments(int argc, char** argv);

        // Finish up arguments by verifying that the columns
        // meet criteria for each column.
        bool argumentCleanup ();

        // Getters
        int returnNumberOfAnnotationFiles ();

        //
        // Class Public Variables
        //
        // Column number (base 1) where the chromosomes/segments are recorded
        int chromosomeColumn;
        // Column number (base 1) where the start values are recorded
        int startColumn;
        // Column number (base 1) where the end values are recorded
        int endColumn;
        // Last column out of chromosome, start, and end for quicker reading
        int lastColumn;
        // How many annotation files have been entered in
        int annotationFileNumber;
	int nucleotideColumn;

        unsigned int threads_to_use;
        const unsigned int maximum_threads;

        // Whether the files have a header or not
        bool header;
        bool zeroBased;
	bool nucleotides;

        // Allowed file types (i.e. csv, tsv, etc.)
        std::string fileType;
        // List of annotation files
        std::vector <std::string> annotationFiles;
        // vector of threads that are launched
        std::vector <std::string> threads;
        // List of the output data files (one for each chromosome)
        std::vector <std::string> dataFiles;
        // Dictionary of the reference IDs with the number of files that matched them
        std::map <std::string, int> referenceIDs;
        // Dictionary of the header rows from each file
        // Position here matches file position in annotationFiles
        std::map <std::string, std::string> headerRows;

        // Enum of different mapping styles possible
        CrossMapType mappingStyle;

        // Debugging
        #ifdef DEBUG
            void printFiles ();
            void printClassVariables ();
        #endif

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
