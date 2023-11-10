#!/bin/bash
# Lokitiedoston lyhentäminen
#
FILENAME=log.txt
SIZE=$(stat -c%s "$FILENAME")
if [ $SIZE -gt 10000 ]
then
	mv $FILENAME $FILENAME.old
	tail -n 100 $FILENAME.old > $FILENAME
fi
