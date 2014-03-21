#!/bin/bash

clear;

if (g++ -Wall -pedantic `pkg-config --cflags opencv` -o `basename $1 .cpp` $1 `pkg-config --libs opencv`) then
	echo "### COMPILOU ###";
fi
