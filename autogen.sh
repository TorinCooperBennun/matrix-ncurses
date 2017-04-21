#!/bin/sh
mkdir -p out
cd out
rm -rf *
CC=clang CXX=clang++ cmake ..
