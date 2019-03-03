#!/bin/bash
cd build
NOW=$(date '+%F_%H_%M_%S')
filename="../risultati/test_$NOW"
# for i in 90 95 45 65 70 75
# do
for (( i=5 ; i<=95 ; i+=5)); do
  ./testing $i 10 $filename
  echo "Script sleeps."
  sleep $[ ( $RANDOM % 30 )  + 1 ]s
done
