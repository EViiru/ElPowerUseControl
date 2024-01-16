#!/bin/bash
# Lokitiedoston lyhentäminen
#
FILENAME=log.txt
SIZE=$(stat -c%s "$FILENAME")
if [ $SIZE -gt 10000 ] # Maksimikoko, bytes
then
	cp $FILENAME $FILENAME.tmp
	tail -n 100 $FILENAME.tmp > $FILENAME # Lyhennetty, riviä
	rm $FILENAME.tmp
	date >> $FILENAME
	echo "Run logcut.sh" >> $FILENAME
fi
