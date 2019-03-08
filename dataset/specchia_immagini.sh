#!/bin/bash

for (( i=1 ; i<=118 ; i+=1)); do
  filename="G12/$i-cont.bmp"
  convert -flop $filename $filename
  convert -flip $filename $filename
done
for (( i=1 ; i<=112 ; i+=1)); do
  filename="G13/$i-cont.bmp"
  convert -flop $filename $filename
  convert -flip $filename $filename
done
for (( i=1 ; i<=100 ; i+=1)); do
  filename="G14/$i-cont.bmp"
  convert -flop $filename $filename
  convert -flip $filename $filename
done
