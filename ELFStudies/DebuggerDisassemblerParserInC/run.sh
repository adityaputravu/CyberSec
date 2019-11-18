#!/usr/bin/env bash

$PWD/compile.sh

gcc -L$PWD/bin/ -Wl,-rpath=$PWD/bin/ -o $PWD/bin/main $PWD/src/main.c -ldebugger -lstrings -lheader && $PWD/bin/main $PWD/sample/new
