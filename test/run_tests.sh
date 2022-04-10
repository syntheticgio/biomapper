#!/bin/bash


# OVERLAP TESTS
../BioMapper --annotations testfile1.csv testfile2.csv --output test_output_overlap.csv --map_type OVERLAP --chromosome 1 --start 2 --end 3
# Overlap Multithread Test
../BioMapper --annotations testfile1.csv testfile2.csv --output test_output_overlap_multithread.csv --map_type OVERLAP --chromosome 1 --start 2 --end 3 --cpus 4

# EXCLUSIVE TESTS
../BioMapper --annotations testfile1.csv testfile2.csv --output test_output_exclusive.csv --map_type EXCLUSIVE --chromosome 1 --start 2 --end 3
# Exclusive Multithrad Test
../BioMapper --annotations testfile1.csv testfile2.csv --output test_output_exclusive.csv --map_type EXCLUSIVE --chromosome 1 --start 2 --end 3 --cpus 4

# COLLAPSE TESTS
../BioMapper --annotations testfile1.csv testfile2.csv --output test_output_collapsed.csv --map_type COLLAPSE --chromosome 1 --start 2 --end 3
# Collapsed multithread test
../BioMapper --annotations testfile1.csv testfile2.csv --output test_output_collapsed.csv --map_type COLLAPSE --chromosome 1 --start 2 --end 3 --cpus 4