#!/bin/bash
COUNTER=2
while [  $COUNTER -lt 18 ]; do
  echo The counter is $COUNTER
  wget http://www.intuit.ru/dnld/7054/video/665/$COUNTER.avi
  let COUNTER=COUNTER+1
done
