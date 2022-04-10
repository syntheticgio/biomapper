#!/usr/bin/env python
import os
import cffi

class Mapper:
    def __init__(self):
        ffi = cffi.FFI()
    
        ffi.cdef(
            "void cffi_test_bindings(int test);"
        )
        self.C = ffi.dlopen("./libmapper.so")

    def test_bindings(self, test: int):
        self.C.cffi_test_bindings(test) 


if __name__ == "__main__":
    ffi = cffi.FFI()
    
    # with open(os.path.join(os.path.dirname(__file__), "mapper.hpp")) as f:
    #     ffi.cdef(f.read())
    ffi.cdef(
        "void cffi_test_bindings(int test);"
    )
    # C = ffi.dlopen("./libmapper.so")

    # def test_bindings(test: int):
    #     C.cffi_test_bindings(test)

    # test_bindings(4)

    ffi.set_source("_mapper",
        # Since we are calling a fully built library directly no custom source
        # is necessary. We need to include the .h files, though, because behind
        # the scenes cffi generates a .c file which contains a Python-friendly
        # wrapper around each of the functions.
        '#include "mapper.hpp"',
        # The important thing is to include the pre-built lib in the list of
        # libraries we are linking against:
        libraries=["mapper"],
        library_dirs=[os.path.dirname(__file__),],
    )

    ffi.compile()