#!/bin/bash
set -o nounset
FILE=$1
echo "Converting $FILE from iso-8859-1 to utf-8"
(iconv -f iso-8859-1 -t utf-8 $FILE > $FILE.conv) && (mv $FILE.conv $FILE)
