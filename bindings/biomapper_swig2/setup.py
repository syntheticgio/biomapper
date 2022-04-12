#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension


mapper_module = Extension('_mapper',
                           sources=['mapper_wrap.cxx', 'mapper.cpp'],
                           )

setup (name = 'mapper',
       version = '0.1',
       author      = "John Torcivia",
       description = """BioMapper annotation mapper for biological applications.""",
       ext_modules = [mapper_module],
       py_modules = ["mapper"],
       )