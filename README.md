# biomapper
##Command line software that enables two biological annotations to be mapped together for overlaps and exclusions.

#WARNING: THIS SOFTWARE IS STILL IN DEVELOPMENT AND MAY NOT FUNCTION PROPERLY UNTIL THIS WARNING IS REMOVED.

###TODO:
- [ ] Multithreading
  - [x] Add multithreading capability
  - [X] Allow user to select number of threads up to maximum
  - [ ] Verify multithreading works on larger data sets
  - [ ] Lock global mutex or BioMapper class mutex?  Check to see if class mutex suffices.
- [ ] Output
  - [ ] Generate generic text readable output files (currently in binary) based on the files mapped
  - [ ] Specific flags can be provided for special mapping cases (i.e. SNVs vs. SNV dataset for exclusions, etc.)
- [x] Enable EXCLUSIVE (in first annotation but not in the second)
- [ ] Enable COLLAPSE (record all non-redundant positions)
- [ ] Enable VCF file_type
- [ ] Enable GTF/GFF file_type
- [x] Headers
  - [x] Record headers in vector
  - [x] Record headers in map instead?
- [ ] SNPs specific comparison
  - [x] Add SNPS flag
  - [ ] Record variant nucleotides in 2 bit format
  - [ ] On merging bitmaps for position; check nucleotides at associated position
  
  

###USAGE:


biomapper --annotations <file1.csv> <file2.csv> <...> [--map_type <overlap>] [--chromosome <1>] [--start <2>] [--end <2>] [--file_type <csv>] [--zero_based] [--no_header] [--cpus <threads>] [--snps]



REQUIRED FLAGS:

    --annotations 	/ -a    :	The files, one after another, must be entered after this flag.  They must all be in the same format currently.

OPTIONAL FLAGS:

	--map_type 	/ -m	:	This is the type of mapping the biomapper should perform.  Current options are OVERLAP (returns loci that exist in all annotation files), EXCLUSIVE (returns loci that exist in only in any one of the annotations) and COLLAPSE (returns loci that exist in any annotations).
	--chromosome 	/ -c 	:	This is the column number (begining at 1) where the chromosome or reference ID information is in each annotation file.  All files must have the same column as the chromosome information currently.  The default for this value is column 1.
	--start 	/ -s   	:	This is the column number (beginning at 1) where the start position for the loci is recorded in the annotation file.  All files must have the same column as the start position.  The default for this value is column 2.
	--end 		/ -e	:	This is the column number (beginning at 1) where the end position for the loci is recorded in the annotation file.  All files must have the same column as the end position.  If the end position column is the same as the start position column, it signifies that the loci are for a single position (SNP for example).  This default for this value is column 2, making the default position information single position if neither flag is set.
	--file_type 	/ -f	:	This flag sets the separator value for the file.  Currently supported are CSV (the default) and TSV.  This allows the program to determine the different columns in your data.
	--zero_based 	/ -z	:	This flag sets the annotation files to be 0 based instead of 1 based (i.e. the first position is 0 instead of 1).  The default (if the flag is absent) is that the ranges in the files start at 1.  For example, the first position in Chromosome 2 is generally considered 1, not 0.  However it is possible to use a system where the first position is 0 instead of 1.
	--no_header 	/ -h	:	This flag turns off headers in the annotation files.  If your files have no header lines (only data) this flag should be set.
	--cpus 		/ -t	:	This flag sets the number of threads to use (up to the maximum determined by std::thread::hardware_concurrency().  The number should follow the flag.  If this flag is not set, the program will default to one thread (not mutlithreaded).
	--snps 		/ -p	:	This flag informs the program that all files entered are SNP/SNV files (only recording single positions, and not ranges).  Following this flag, put the column number that the variant nucleotide (SNP) is in.  The SNPS flag is not needed even if you're using SNPs unless you want special functionality to compare the variants to either match (OVERLAP) or not match (EXCLUSIVE)
