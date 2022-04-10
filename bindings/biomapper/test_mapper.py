#!/usr/bin/env python

from mapper import MapType, Mapper

my_mapper = Mapper

reg_file1 = my_mapper.add_file(file_name="testfile1.csv", map_column=1, start_index_column=2, end_index_column=3)
# TODO: check return here
reg_file2 = my_mapper.add_file(file_name="testfile2.csv", map_column=1, start_index_column=2, end_index_column=3)
# TODO: check return here

print(my_mapper.current_file_list())