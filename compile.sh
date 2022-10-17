#!/bin/sh
g++ -o ./bin/run.out ./src/*.cpp main.cpp  -lcrypto
g++ -o ./bin/tests.out ./src/*.cpp tests.cpp -lcrypto