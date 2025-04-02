#!/bin/sh
g++ -c main.cpp
g++ main.o -o hoi -lsfml-graphics -lsfml-window -lsfml-system
rm main.o
./hoi