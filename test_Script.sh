#!/bin/bash
###g++ bankers.cpp
###./a.out $1 $2
touch output.txt
sh 20172123.sh input_test_file.txt output.txt

file1="output.txt"
file2="output_test_file.txt"

if cmp -s "$file1" "$file2"; then
    printf 'The program is correct.'
else
    printf 'The program is flawed.'
fi
