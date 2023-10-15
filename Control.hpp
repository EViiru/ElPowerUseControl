// Ohjauslogiikka

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>

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


using namespace std;

class Control {

	public:
		Control();
		~Control();
		int cntrlOn(); // Ohjaus käynnissä
		int cntrlOff(); // Ohjaus pysähdyksissä
		
	private:
		// Ohjausrajat (oletusarvot)
		array<float,2> winOut = {-9999, 2}; // Ulostulo
		array<float,2> winGreen = {-9999, 5}; // Vihreä LED
		array<float,2> winYellow = {2.5, 10}; // Keltainen LED
		array<float,2> winRed = {7.5, 9999}; // Punainen LED

		time_t lastUpdate = 0; // Edellinen hintojen haku
		time_t const UPDATE_TIME = 12 * 3600; // Hintojen päivitysväli
		string const SHELLYPLUGS_IP = "http://192.168.10.37/"; // ShellyPlugS IP-osoite

};
