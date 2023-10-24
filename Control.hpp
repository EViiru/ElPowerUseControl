// Ohjauslogiikka

#ifndef SPOTPRICES_HPP
#define SPOTPRICES_HPP
#include "SpotPrices.hpp"
#endif

#ifndef RASPIIO_HPP
#define RASPIIO_HPP
#include "RasPiIO.hpp"
#endif

#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include "Settings.hpp"
#endif

#ifndef SHELLYPLUGS_HPP
#define SHELLYPLUGS_HPP
#include "ShellyPlugS.hpp"
#endif

#include <iostream>
using std::cout;
using std::endl;
#include <array>
using std::array;

using std::string;

/**
\brief Ohjaustoiminnot
*
* Ohjaustoimintojen pääohjelma, jossa on kaksi tilaa:
* + ohjaus sähkönhintojen mukaan on käynnissä ja
* + ohjaus on pois päältä.<p>
* Tilanvaihto menee pääohjelman kautta. Kun käynnissä oleva tila havaitsee tilamuutospyynnön, se keskeyttää toimintansa ja kontrolli palaa pääohjelmaan. 
* Pääohjelma käynnistää sitten toisen tilan.@n
* Ohjelmassa ei ole hallittua pysäytystä, vaan sen on tarkoitus olla käynnissä aina, kun virta on kytkettynä. Tämä varmistetaan automaattisella 
* käynnistymisellä (virtakatkon jälkeen) ja vahtikoiratoiminnolla, joka uudelleenkäynnistää ohjauslaitteen ohjelmakierron pysähtyessä. (Vahtikoiratoiminto 
* otetaan käyttöön vasta "tuotantolaitteissa".) 
*/
class Control {

	public:
/**
* Rakentajassa ei ole toimintoja.
*/
		Control();
/**
* Purkajassa ei ole toimintoja.
*/
		~Control();
/**
* Ohjaus sähkönhintojen mukaan on käynnissä.
* \return 0
*/
		int cntrlOn();
/**
* Ohjaus ei ole käynnissä. Ainoastaan vahtikoiran syöttäminen ja tilamuutospyynnön seuranta ovat toiminnassa.
* \return 0
*/
		int cntrlOff();
		
	private:
		// Ohjausrajat (oletusarvot)
		array<float,2> winOut_ = {-9999, 2}; /**< Ohjausrajat, ulostulo */
		array<float,2> winGreen_ = {-9999, 5}; /**< Ohjausrajat, vihreä LED */
		array<float,2> winYellow_ = {2.5, 10}; /**< Ohjausrajat, keltainen LED */
		array<float,2> winRed_ = {7.5, 9999}; /**< Ohjausrajat, punainen LED */

		time_t nextUpdate_ = 0; /**< Seuraava hintojen haku */
		time_t const UPDATE_TIME = 8 * 3600; /**< Hintojen päivitys, aika ennen edellisten tietojen loppumista */
		string const SHELLYPLUGS_IP = "http://192.168.10.37/"; /**< ShellyPlugS IP-osoite */
		string const FILENAME_PRICES = "ElPrices_.csv"; /**< Tiedostonimen vakio-osa: Sähkön hinnat */
		string const FILENAME_SETTINGS = "ohjausrajat.txt"; /**< Tiedostonimi: Asetukset */
		
		Control(Control const& src); /**< Kopiorakentaja (oletus) estetty */
		Control& operator=(Control const& src); /**< Sijoitus (oletus) estetty */

};
