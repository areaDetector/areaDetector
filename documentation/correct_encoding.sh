#!/bin/bash
set -o nounset
FILE=$1
#(iconv -f utf-8 -t utf-8 $FILE > $FILE.conv) && (mv $FILE.conv $FILE)
sed -i 's/[\d128-\d255]//g' $FILE
