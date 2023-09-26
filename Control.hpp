// Ohjauslogiikka

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>

#ifndef SPOTPRICES
#define SPOTPRICES
#include "SpotPrices.hpp"
#endif

#ifndef RASPIIO
#define RASPIIO
#include "RasPiIO.hpp"
#endif

using namespace std;

class Control {

	public:
		Control();
		~Control();
		int cntrlLoop(); // Ohjaus
		
	private:
		// Ohjausrajat
		array<float,2> winOut = {-9999, 0.3}; // Ulostulon ohjaus
		array<float,2> winGreen = {-9999, 5}; // Vihreä LED
		array<float,2>  winYellow = {5, 10}; // Keltainen LED
		array<float,2>  winRed = {10, 9999}; // Punainen LED

		SpotPrices *sp;
		time_t lastUpdate = 0; // Edellinen hintojen haku
		const time_t updateTime = 12 * 3600; // Hintojen päivitysväli

};
