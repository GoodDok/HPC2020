#!/bin/bash
set -e
set -x

make clean all 
./matrix_mult.exe
