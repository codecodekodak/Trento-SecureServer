#!/bin/bash

URL=$1
METHOD="GET"
COOKIE=""
DATA='pass=bb&drop=register&amount=666'




echo $URL, $METHOD, $DATA

while :
do
    RAND_USER="&user=user$RANDOM"
    echo "$URL?$DATA$RAND_USER"
    curl -q -X $METHOD "$URL?$DATA$RAND_USER" 2>/dev/null 
done

