#!/bin/bash

swig -c++ -python mapper.i
# Using SWIG
python setup.py build_ext --inplace

# Build by hand
#g++ -shared -fPIC mapper.cpp mapper_wrap.cxx -I/usr/include/python3.9 -o mapper.o




#### ARCHIVE
#g++ -shared -Wl,-soname,libmapper.so -o libmapper.so  mapper.o
