#!/bin/bash
# Käynnistää ohjauksen ja web-käyttöliittymän tausta-ajoon
# Huom. Web-portti 8000 -> selaimen osoiterivin loppuun :8000
#
# Varmistetaan tarvittavien alihakemistojen oleminen
DIR=./temp
if [ ! -d $DIR ];
then
	mkdir $DIR
fi
DIR=./data
if [ ! -d $DIR ];
then
	mkdir $DIR
fi
# Käynnistetään ohjelmat
./ElUseCntrl >> log.txt &
gunicorn -w 2 --bind 0.0.0.0:8000 RasPiUI:app &
