#!/bin/bash

set -e

rm -f ./bin/*

for file in ./src/*.cpp; do
	echo "Compiling \`$file\`"
	g++ -c $file -o "./bin/$(basename $file .cpp).o" $@
done

echo "Compiling \`./main.cpp\`"
g++ main.cpp ./bin/*.o -o "./bin/main.out" -l crypto -l jsoncpp $@

echo "Compiling \`./tests.cpp\`"
g++ tests.cpp ./bin/*.o -o "./bin/tests.out" -l crypto -l jsoncpp $@
