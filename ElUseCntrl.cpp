/************************************************************************

Sähkönkulutuksen ohjaus

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

************************************************************************/

#ifndef CONTROL_HPP
#define CONTROL_HPP
#include "Control.hpp"
#endif

/**
* \mainpage
* Ohjelma hakee (pörssi)sähkön hintatiedot API-rajapinnasta: "https://porssisahko.net/api" ja tallentaa ne levylle historiatiedoksi.@n
* Sähkön hinnan ollessa asetuksissa määritettyä rajaa halvempi, kytketään Raspberry Pi:n ohjauslähtö ja wifi-ohjattava pistorasia päälle.@n
* <p>
* Ohjauslähdön lisäksi Raspberry Pi:n lähdöissä on varaukset merkkivaloille: vihreä, keltainen ja punainen. Ohjauslähdön ja merkkivalojen 
* kytkentärajat määritetään asetustiedostossa: "settings.json".@n
* <p>
* Ohjelmakierrossa on kaksi tilaa:
* + sähkönhinnan seuranta ja sen mukaan ohjaus ovat päällä ja
* + ohjaus on pois päältä.@n
* <p>
* Tilaa vaihdetaan kirjoittamalla työhakemistoon tiedosto:
* + stop, kun halutaan ohjaus pois päältä tai
* + start, kun halutaan hintaseuranta ja sen mukaan ohjaus päälle.@n
* <p>
* Vaihdettaessa tilaa stop -> start, ohjelma lukee asetusarvot  tiedostosta "settings.json".@n
* Jos työhakemistossa on tiedosto "update", ohjelma lukee (uudet) asetusarvot  tiedostosta "settings.json".@n
* <p>
* Ohjelmalle on Python Flask'illa tehty web-käyttöliittymä: RasPiUI.py.
*/
int main() {
	int ret;
	Control ctrl; // Ohjauslogiikka
	while(1) { // Tilakone
		ret = ctrl.cntrlOn(); // Käynnissä
		ret = ctrl.cntrlOff(); // Pysähdyksissä 
	}
	return 0;
}