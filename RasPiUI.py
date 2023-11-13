## @package RasPiUI Käyttöliittymä, sähkönkulutuksen ohjaus
#
# Web-käyttöliittymä Raspberry Pi-korttitietokoneessa toimivalle sähkönkulutuksen ohjaukselle.
# Ohjelmointiympäristö Python Flask
#
# Käyttöliittymässä on kolme sivua:
# * Ohjauksen tilan seuranta
# * Ohjauksen tilan ja asetusten muutos
# * Tietoja ohjelmasta

"""
***********************************************************************

Sähkönkulutuksen ohjaus, käyttöliittymä

Ohjelma ohjaa sähkönkulutusta pörssisähkön hinnan perusteella.
Laitealusta: Raspberry Pi

Copyright (C) 2023  Esko Viiru

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

***********************************************************************
"""

import os, json
from flask import Flask, request, render_template, redirect, url_for

## Flask-sovellus
#
# \var app
# + Flask-sovellus
# \var debug
# + True: testaus
# + False: käyttö
# \var port
# + Kuunneltava portti
# \var host
# + Kuunneltava IP
app = Flask(__name__)

# Ohjauksen seuranta, aloitusnäyttö
@app.route("/")
## Tilanäyttö
#
# Sivulla näytetään:
# * Ohjauksen tila: käynnissä / pysähdyksissä
# * voimassaolevat asetukset ja
# * lokitiedosto@n
# <p>Tilaa ja asetuksia ei voi muuttaa.
def monitor():
	run = os.path.exists("temp/controlOn") # Ohjaus käynnissä
	if os.path.exists("settings.json"):	
		with open("settings.json", "r", encoding="utf-8") as f: # Asetukset
			settings = json.loads(f.read())
	else:
		settings = {} # Jos tiedostoa ei ole	
	if os.path.exists("temp/outputs.json"):	
		with open("temp/outputs.json", "r", encoding="utf-8") as f: # Lähdöt
			outputs = json.loads(f.read())
	else:
		outputs = {} # Jos tiedostoa ei ole
	if os.path.exists("log.txt"):	
		with open("log.txt", "r", encoding="utf-8") as f: # Lokitiedosto
			log = f.readlines()
			log.reverse() # Lopusta alkuun
	else:
		log = [] # Jos tiedostoa ei ole
	
	return render_template("monitor.html", run=run, settings=settings, outputs=outputs, log=log)
 
    
## Asetusten muutos, aloitusnäyttö
#
# Sivulla näytetään:
# * Ohjauksen tila: käynnissä / pysähdyksissä ja
# * voimassaolevat asetukset@n
# <p>Tilaa ja asetuksia voidaan muuttaa.
@app.route("/settings")
def settings():
	run = os.path.exists("temp/controlOn") # Ohjaus käynnissä	
	if os.path.exists("settings.json"):	
		with open("settings.json", "r", encoding="utf-8") as f: # Asetukset
			settings = json.loads(f.read())
	else:
		settings = {"Output": [-9999.0, 2.5]} # Oletusarvot, jos tiedostoa ei ole
		settings["Green"] = [-9999.0, 5.0]
		settings["Yellow"] = [2.5, 10.0]
		settings["Red"] = [7.5, 9999.0]	
	return render_template("settings.html", run=run, settings=settings)


## Asetusten muutos, uudet asetukset
#
# Tallennetaan muutetut asetukset ja tehdään työhakemistoon tiedosto "update".
@app.route("/saveSettings", methods = ["GET", "POST"])
def saveSettings():
	if request.method == "POST":
		newSet = {"Output": [float(request.form.get("OutputAla")), float(request.form.get("OutputYla"))]}
		newSet["Green"] = [float(request.form.get("GreenAla")), float(request.form.get("GreenYla"))]
		newSet["Yellow"] = [float(request.form.get("YellowAla")), float(request.form.get("YellowYla"))]
		newSet["Red"] = [float(request.form.get("RedAla")), float(request.form.get("RedYla"))]
	
	if os.path.exists("settings.json"):	
		with open("settings.json", "r", encoding="utf-8") as f: # Asetukset, vanhat
			settings = json.loads(f.read())
	else:
		settings = {} # Jos tiedostoa ei ole	

	if settings != newSet: # Tiedot muuttuneet
		with open("settings.json", "w", encoding="utf-8") as f:
			f.write(json.dumps(newSet, indent=4))
		with open("update", "w") as f:
			f.write("")
			
	return redirect(url_for("monitor"))


## Asetusten muutos, käynnistys / pysäytys
#
# Tehdään työhakemistoon tiedosto "start" tai "stop".
@app.route("/startStop", methods = ["GET", "POST"])
def startStop():
	if request.method == "POST":
		startStop = request.form.get("nappi")
		if startStop == "Start": # Käynnistys
			if os.path.exists("stop"):
				os.remove("stop")
			with open("start", "w") as f:
				f.write("")
		else: # Pysäytys
			if os.path.exists("start"):
				os.remove("start")
			with open("stop", "w") as f:
				f.write("")			

	return redirect(url_for("monitor"))


## Tietoja ohjelmasta
#
# Lyhyt kuvaus ohjelmasta ja ohjelman versio(t)
@app.route("/about")
def about():
	return render_template("about.html")

if __name__ == "__main__":
	app.run(debug=True, port=8000, host="0.0.0.0")
