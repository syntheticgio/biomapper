# biomapper
##Command line software that enables two biological annotations to be mapped together for overlaps and exclusions.

#WARNING: THIS SOFTWARE IS STILL IN DEVELOPMENT AND MAY NOT FUNCTION PROPERLY UNTIL THIS WARNING IS REMOVED.

###TODO:
- [ ] Multithreading
  - [x] Add multithreading capability
  - [ ] Allow user to select number of threads up to maximum
  - [ ] Verify multithreading works on larger data sets
- [ ] Output
  - [ ] Generate generic text readable output files (currently in binary) based on the files mapped
  - [ ] Specific flags can be provided for special mapping cases (i.e. SNVs vs. SNV dataset for exclusions, etc.)
- [ ] Enable EXCLUSIVE (in first annotation but not in the second_

###USAGE:


biomapper --annotations <file1.csv> <file2.csv> <...> [--map_type <overlap>] [--chromosome <1>] [--start <2>] [--end <2>] [--file_type <csv>] [--zero_based] [--no_header] 



REQUIRED FLAGS:

    --annotations 	/ -a    :    The files, one after another, must be entered after this flag.  They must all be in the same format currently.
    
OPTIONAL FLAGS:
  
	--map_type 		/ -m	:	This is the type of mapping the biomapper should perform.  Current options are OVERLAP (returns loci that exist in all annotation files) and EXCLUSIVE (returns loci that exist in the first annotation file but NOT any subsequent ones).
	--chromosome 	/ -c 	:	This is the column number (begining at 1) where the chromosome or reference ID information is in each annotation file.  All files must have the same column as the chromosome information currently.  The default for this value is column 1.  
	--start 		/ -s   	:	This is the column number (beginning at 1) where the start position for the loci is recorded in the annotation file.  All files must have the same column as the start position.  The default for this value is column 2.
	--end 			/ -e	:	This is the column number (beginning at 1) where the end position for the loci is recorded in the annotation file.  All files must have the same column as the end position.  If the end position column is the same as the start position column, it signifies that the loci are for a single position (SNP for example).  This default for this value is column 2, making the default position information single position if neither flag is set.
	--file_type 	/ -f	:	This flag sets the separator value for the file.  Currently supported are CSV (the default) and TSV.  This allows the program to determine the different columns in your data.
	--zero_based 	/ -z	:	This flag sets the annotation files to be 0 based instead of 1 based (i.e. the first position is 0 instead of 1).  The default (if the flag is absent) is that the ranges in the files start at 1.  For example, the first position in Chromosome 2 is generally considered 1, not 0.  However it is possible to use a system where the first position is 0 instead of 1.
	--no_header 	/ -h	:   This flag turns off headers in the annotation files.  If your files have no header lines (only data) this flag should be set.
