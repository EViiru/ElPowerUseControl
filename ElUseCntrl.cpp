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
* kytkentärajat määritetään asetustiedostossa: "ohjausrajat.txt".@n
* <p>
* Ohjelmakierrossa on kaksi tilaa:
* + sähkönhinnan seuranta ja sen mukaan ohjaus ovat päällä ja
* + ohjaus on pois päältä.@n
* <p>
* Tilaa vaihdetaan kirjoittamalla työhakemistoon tiedosto:
* + stop, kun halutaan ohjaus pois päältä tai
* + start, kun halutaan hintaseuranta ja sen mukaan ohjaus päälle.@n
* <p>
* Vaihdettaessa tilaa stop -> start, ohjelma lukee (uudet) asetusarvot  tiedostosta "ohjausrajat.txt".
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