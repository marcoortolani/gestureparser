#!/bin/bash
cd build
NOW=$(date '+%F_%H_%M_%S')
filename="../risultati/test_$NOW"
# for i in 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95
# do
for (( i=5 ; i<=85 ; i+=5)); do
  seed=$(date "+%m%d%y%H%M%S")
  echo "./testing $i 10 $filename $seed"
  ./testing $i 10 $filename $seed
  echo "Script sleeps."
  sleep $[ ( $RANDOM % 30 )  + 1 ]s
done
