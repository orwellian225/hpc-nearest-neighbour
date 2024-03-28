#!/bin/sh

cd build
make
cd ..
cp build/knn ./knn
