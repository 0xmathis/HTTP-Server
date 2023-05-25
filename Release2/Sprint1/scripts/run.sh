#!/bin/bash

export LD_LIBRARY_PATH=$PWD/bin

if [[ $# -eq 1 && "$1" == "valgrind" ]]; then
  valgrind --track-origins=yes -s --leak-check=full --show-leak-kinds=all ./bin/server
else
  ./bin/server
fi
