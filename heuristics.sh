#!/bin/bash

URL=http://localhost:8080/process.php
METHOD="GET"
COOKIE=""
DATA='pass=pass&amount=1111'




echo $URL, $METHOD, $DATA

while :
do
    PAYLOAD="&user=test$RANDOM"
    echo Index : $(time curl -v -X $METHOD "localhost:8080" >/dev/null 2>/dev/null)
    echo Register : $(time curl -v -X $METHOD "$URL?$DATA$PAYLOAD&drop=register" >/dev/null 2>/dev/null)
    echo Balance : $(time curl -v -X $METHOD "$URL?$DATA$PAYLOAD&drop=balance" >/dev/null 2>/dev/null)
    echo Deposit : $(time curl -v -X $METHOD "$URL?$DATA$PAYLOAD&drop=deposit" >/dev/null 2>/dev/null)
    echo Withdraw : $(time curl -v -X $METHOD "$URL?$DATA$PAYLOAD&drop=withdraw" >/dev/null 2>/dev/null)
    sleep 3;
done
