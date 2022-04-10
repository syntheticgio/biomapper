# biomapper
##Command line software that enables two biological annotations to be mapped together for overlaps and exclusions.

#WARNING: THIS SOFTWARE IS STILL IN DEVELOPMENT AND MAY NOT FUNCTION PROPERLY UNTIL THIS WARNING IS REMOVED.

###TODO:
- [ ] Multithreading
  - [x] Add multithreading capability
  - [x] Allow user to select number of threads up to maximum
  - [ ] Verify multithreading works on larger data sets
  - [ ] Lock global mutex or BioMapper class mutex?  Check to see if class mutex suffices.
- [ ] Output
  - [x] Generate generic text readable output files (currently in binary) based on the files mapped
  - [ ] Specific flags can be provided for special mapping cases (i.e. SNVs vs. SNV dataset for exclusions, etc.)
- [x] Enable EXCLUSIVE (in first annotation but not in the second)
- [x] Enable COLLAPSE (record all non-redundant positions)
- [ ] Enable VCF file_type
- [ ] Enable GTF/GFF file_type
- [x] Enable output file flag
- [x] Headers
  - [x] Record headers in vector
  - [x] Record headers in map instead?
- [ ] SNPs specific comparison
  - [x] Add SNPS flag
  - [ ] Record variant nucleotides in 2 bit format
  - [ ] On merging bitmaps for position; check nucleotides at associated position

###BUILDING:

Biomapper can be built in the following ways:

####CMAKE
Debug build:
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

Release build:
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

####Make
```
	COMMAND			:	BINARY FILE IN SRC		:	COMMENT
	-------			:	------------------		:	-------
	make debug		:	biomapper-linux-debug	:	This builds the linux debug build of biomapper.  This enables additional output to the console including debugging information.  It is recommended not to use this build unless you are troubleshooting the program.
	make release	: 	biomapper-linux			:	This builds the linux release version of biomapper.  This is the fastest version and should be used on a Linux system.  This was tested on a Linux Mint 64 bit system but should work on any Linux build that supports c++11.
	make osx_debug	:	biomapper-osx-debug		:	This builds an OSX compatible debug version of biomapper (tested on OSX El Capitan).  This enables additional output to the console including debugging information.  It is recommended not to use this build unless you are troubleshooting the program.
	make osx		:	biomapper-osx			:	This builds an OSX compatible version of biomapper (tested on OSX El Capitan).  This is the version that should be used on an OSX system and offers compatibility with the clang native compiler.
	make raspberry  :	biomapper-raspbian		:	This builds a Raspbian (default raspberry pi version of linux) release version of biomapper.
	make clean		:	N/A						:	This will clean binary and supporting files from the biomapper/bin/ directory.  This should be done before re-compiling a new binary.
```

Most users interested in this software are using a Linux based system and should compile an appropriate binary by typing 'make release'. This will place the biomapper-linux binary in the biomapper/bin/ directory.  In order to install it for system wide usage, you should copy this binary to your user bin directory (typically /usr/bin/).

Users on Mac OSX systems should generate the binary file by typing 'make osx' in the terminal within the biomapper directory.  The binary file biomapper-osx will be found in the biomapper/bin/ directory.

###USAGE:


biomapper --annotations <file1.csv> <file2.csv> <...> [--output <output_file_name.csv>] [--map_type <overlap>] [--chromosome <1>] [--start <2>] [--end <2>] [--file_type <csv>] [--zero_based] [--no_header] [--cpus <threads>] [--snps]



REQUIRED FLAGS:

    --annotations 	/ -a    :	The files, one after another, must be entered after this flag.  They must all be in the same format currently.

OPTIONAL FLAGS:

	--output / -o		:	This sets the output file name.  If the directory is not provided it will be created in the working directory.  
                                    If no parameter is entered, the default file name will be 'output.csv'.
	--map_type / -m		:	This is the type of mapping the biomapper should perform.  Current options are OVERLAP (returns loci 
                                    that exist in all annotation files), EXCLUSIVE (returns loci that exist in only in any one of the annotations) 
                                    and COLLAPSE (returns loci that exist in any annotations).
	--chromosome / -c 	:	This is the column number (begining at 1) where the chromosome or reference ID information is in each 
                                    annotation file.  All files must have the same column as the chromosome information currently.  
                                    The default for this value is column 1.
	--start / -s   		:	This is the column number (beginning at 1) where the start position for the loci is recorded in the 
                                    annotation file.  All files must have the same column as the start position.  The default for this value is 
                                    column 2.
	--end / -e		:	This is the column number (beginning at 1) where the end position for the loci is recorded in the 
                                    annotation file.  All files must have the same column as the end position.  If the end position column is 
                                    the same as the start position column, it signifies that the loci are for a single position (SNP for example).  
                                    This default for this value is column 2, making the default position information single position if neither flag is set.
	--file_type / -f	:	This flag sets the separator value for the file.  Currently supported are CSV (the default) and TSV.  
                                    This allows the program to determine the different columns in your data.
	--zero_based / -z	:	This flag sets the annotation files to be 0 based instead of 1 based (i.e. the first position is 0 instead of 
                                    1).  The default (if the flag is absent) is that the ranges in the files start at 1.  For example, the first 
                                    position in Chromosome 2 is generally considered 1, not 0.  However it is possible to use a system where the first 
                                    position is 0 instead of 1.
	--no_header / -h	:	This flag turns off headers in the annotation files.  If your files have no header lines (only data) this flag should 
                                    be set.
	--cpus 	/ -t		:	This flag sets the number of threads to use (up to the maximum determined by std::thread::hardware_concurrency().  
                                    The number should follow the flag.  If this flag is not set, the program will default to one thread (not mutlithreaded).
	--snps 	/ -p		:	This flag informs the program that all files entered are SNP/SNV files (only recording single positions, and not ranges).  
                                    Following this flag, put the column number that the variant nucleotide (SNP) is in.  The SNPS flag is not needed even if 
                                    you're using SNPs unless you want special functionality to compare the variants to either match (OVERLAP) or not match 
                                    (EXCLUSIVE).
