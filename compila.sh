#!/bin/bash

BINDIR=bin

if [ ! -d "$BINDIR" ]; then
  mkdir $BINDIR
fi

cd $BINDIR

# Compila la parte "Kinect"
g++ -std=c++11 -c ../hand-recognizer/Dataset.cpp -o Dataset.o
g++ -std=c++11 -I../libsvm -c ../hand-recognizer/KinectDevice.cpp -o KinectDevice.o
g++ -std=c++11 -c ../hand-recognizer/InputGenerator.cpp -o InputGenerator.o

# Compila la svm
g++ -c ../libsvm/svm.cpp -o svm.o

# Compila nell'eseguibile 'hr'
g++ -std=c++11 -I../hand-recognizer -I../libsvm `pkg-config --cflags --libs opencv` *.o ../hand-main.cpp -o hr
