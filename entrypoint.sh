#!/bin/sh
g++ -o ./bin/run.out ./src/*.cpp main.cpp  -lcrypto
./bin/run.out