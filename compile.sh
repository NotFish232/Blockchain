#!/bin/bash
for file in ./src/*.cpp; do
	g++ -c $file -o "./bin/$(basename $file .cpp).o"
done
g++ main.cpp ./bin/*.o -o ./bin/run.out -l crypto
g++ tests.cpp ./bin/*.o -o ./bin/tests.out -l crypto
