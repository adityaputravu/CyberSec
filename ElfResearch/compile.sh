#!/usr/bin/env bash
gcc -c -fpic $PWD/src/header.c -o $PWD/src/header.o && gcc -shared -o $PWD/bin/libheader.so $PWD/src/header.o

gcc -c -fpic $PWD/src/strings.c -o $PWD/src/strings.o && gcc -shared -o $PWD/bin/libstrings.so $PWD/src/strings.o

gcc -c -fpic $PWD/src/debugger.c -o $PWD/src/debugger.o && gcc -shared -o $PWD/bin/libdebugger.so $PWD/src/debugger.o $PWD/src/strings.o


