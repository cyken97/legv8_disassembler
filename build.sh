#!/bin/bash
g++ -c disasm.cpp -o disasm.o; #
g++ -c main.cpp	-o main.o; #
g++ disasm.o main.o -o disasm; #