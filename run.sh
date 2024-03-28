#!/bin/sh
./compile.sh

mkdir results
touch results/output.csv
./knn | tee results/output.csv
