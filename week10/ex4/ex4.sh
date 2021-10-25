#!/bin/bash
mkdir tmp
echo "hello world" > tmp/file1
echo "hello world 2" > tmp/file2
link tmp/file1 tmp/link1
./ex4 
./ex4 > ex4.txt 2> /dev/null

