#!/usr/bin/env python

import ctypes
import sys
import numpy 

libstrings  = ctypes.cdll.LoadLibrary("bin/libstrings.so")
libdebugger = ctypes.cdll.LoadLibrary("bin/libdebugger.so")

breakpoints = (ctypes.c_int * 1)(*[0x401000])
addr = 0x401000

libdebugger.little_endian_array.restype = ctypes.POINTER(ctypes.c_int)
libdebugger.little_endian_array.argtypes = [ctypes.c_int]
arr = libdebugger.little_endian_array(0x69686766)

print([*map(hex, arr[:32:8])])

libdebugger.attach("sample/new", 0)


