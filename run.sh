#!/bin/sh
g++ -c src/main.cpp -Iheaders -o main.o
g++ main.o -o hoi -lsfml-graphics -lsfml-window -lsfml-system
rm main.o
./hoi