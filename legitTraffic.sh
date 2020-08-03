#!/bin/bash

RANGE=8
while true; do 
	TIME=$(($RANDOM%$RANGE));
	TIME=$(($TIME + 1));
	PAGE="10.1.5.2/index$(($(($RANDOM%10))+1)).html"
	curl -s $PAGE > /dev/null 2>&1 &
	echo $PAGE
	sleep $TIME;
done
