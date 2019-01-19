#!/bin/bash
set -o nounset

BASEDIR=$1
FIND_OPTS="-iname "*.cpp" -o -iname "*.h" -type f"
FILE_OPTS=-i

FILES_UTF8=$(find $BASEDIR $FIND_OPTS -exec file $FILE_OPTS {} \; | grep charset=utf-8 | cut -d":" -f1)
FILES_ISO=$(find $BASEDIR $FIND_OPTS -exec file $FILE_OPTS {} \; | grep charset=iso-8859-1 | cut -d":" -f1)

echo "FILES UTF8"
echo $FILES_UTF8

echo "FILES_ISO"
echo $FILES_ISO

for filename in $FILES_UTF8; do
	echo "Converting $filename from utf-8 to ASCII"
	(iconv -f utf-8 -t ASCII//TRANSLIT $filename > $filename.conv) && (mv $filename.conv $filename)
done

for filename in $FILES_ISO; do
	echo "Converting $filename from utf-8 to ASCII"
	(iconv -f iso-8859-1 -t ASCII//TRANSLIT $filename > $filename.conv) && (mv $filename.conv $filename)
done

# Sanity check

echo "Searching for non ascii files...."
find $BASEDIR $FIND_OPTS -exec file $FILE_OPTS {} \; | grep -v charset=us-ascii
