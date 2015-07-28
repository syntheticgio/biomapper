# biomapper
Command line software that enables two biological annotations to be mapped together for overlaps and exclusions.

USAGE:


biomapper --annotations <file1.csv> <file2.csv> <...> [--map_type <overlap>] [--chromosome <1>] [--start <2>] [--end <2>] [--file_type <csv>]  [--header]



REQUIRED FLAGS:

    --annotations 	/ -a    :    The files, one after another, must be entered after this flag.  They must all be in the same format currently.
    
OPTIONAL FLAGS:
  
    --map_type 		/ -m	:	This is the type of mapping the biomapper should perform.  Current options are OVERLAP (returns loci that exist in all annotation files) and EXCLUSIVE (returns loci that exist in the first annotation file but NOT any subsequent ones).
    --chromosome 	/ -c 	:	This is the column number (begining at 1) where the chromosome or reference ID information is in each annotation file.  All files must have the same column as the chromosome information currently.  The default for this value is column 1.  
    --start 		/ -s   	:	This is the column number (beginning at 1) where the start position for the loci is recorded in the annotation file.  All files must have the same column as the start position.  The default for this value is column 2.
    --end 			/ -e	:	This is the column number (beginning at 1) where the end position for the loci is recorded in the annotation file.  All files must have the same column as the end position.  If the end position column is the same as the start position column, it signifies that the loci are for a single position (SNP for example).  This default for this value is column 2, making the default position information single position if neither flag is set.
	--file_type 	/ -f	:	This flag sets the separator value for the file.  Currently supported are CSV (the default) and TSV.  This allows the program to determine the different columns in your data.
	--header 		/ -h	:   This flag turns on or off there being a header in the annotation file.  If there is a header, it can be safely ignored by selecting this flag.  Otherwise this flag should not be set.
