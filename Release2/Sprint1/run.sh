#!/bin/bash

make
export LD_LIBRARY_PATH=$PWD/bin
echo
./bin/server