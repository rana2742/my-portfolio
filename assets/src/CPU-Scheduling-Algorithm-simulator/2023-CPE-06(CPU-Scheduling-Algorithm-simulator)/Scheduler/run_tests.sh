#!/bin/bash

# Compile the C program first
gcc scheduler.c -o scheduler

# Loop through all test case files
for file in testcases/*.txt
do
    name=$(basename $file .txt)
    ./scheduler fcfs $file results/${name}_fcfs.txt
    ./scheduler sjf  $file results/${name}_sjf.txt
    ./scheduler rr   $file results/${name}_rr.txt 2
done

echo "All tests completed. Check the results folder."
