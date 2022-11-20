#!/bin/bash

set -e

rm -f ./bin/*

size() {
	echo $(du -hs $1 | cut -f1)
}

for file in ./src/*.cpp; do
	base=$(basename $file .cpp)
	g++ -O2 -c $file -o "./bin/$base.o" $@
	echo "Compiled \`$base.cpp\` ($(size ./bin/$base.o))"
done

for file in ./*.cpp; do 
	base=$(basename $file .cpp)
	g++ -O2 -s $file ./bin/*.o -o "./bin/$base.out" -l crypto -l jsoncpp $@ 
	echo "Compiled \`$base.cpp\` ($(size ./bin/$base.out))"
done

echo "Total binary size: $(size ./bin)"
