#!/bin/bash
NOW=$(date '+%F_%H_%M_%S')
filename="../test_$NOW"
#for i in 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95
#do
for (( i=5 ; i<=95 ; i+=5)); do
  ./testing $i $filename
done
