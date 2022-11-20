#!/bin/bash

set -e

rm -f ./bin/*

for file in ./src/*.cpp; do
	base=$(basename $file .cpp)
	g++ -O2 -c $file -o "./bin/$base.o" $@
	echo "Compiled \`$base.cpp\` ($(du -hs ./bin/$base.o | cut -f1))"
done

for file in ./*.cpp; do 
	base=$(basename $file .cpp)
	g++ -O2 -s $file ./bin/*.o -o "./bin/$base.out" -l crypto -l jsoncpp $@ 
	echo "Compiled \`$base.cpp\` ($(du -hs ./bin/$base.out | cut -f1))"
done

echo "Total binary size: $(du -hs ./bin | cut -f1)"
