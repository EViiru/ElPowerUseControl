#!/bin/bash
# Käynnistää ohjauksen ja web-käyttöliittymän tausta-ajoon
# Huom. Web-portti 8000 -> selaimen osoiterivin loppuun :8000
#
./ElUseCntrl >> log.txt &
gunicorn -w 2 --bind 0.0.0.0:8000 RasPiUI:app &

