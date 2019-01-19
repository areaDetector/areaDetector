#!/bin/bash
set -o nounset
FILE=$1
echo "Converting $FILE from iso-8859-1 to utf-8"
(iconv -f iso-8859-1 -t utf-8 $FILE > $FILE.conv) && (mv $FILE.conv $FILE)
echo "Converting $FILE from utf-8 to ASCII"
(iconv -f utf-8 -t ASCII//TRANSLIT $FILE > $FILE.conv) && (mv $FILE.conv $FILE)
