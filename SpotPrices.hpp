// Sähkön hintatiedot

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class SpotPrices {

	public:
		SpotPrices();
		~SpotPrices();
		int updatePrices(); // Hakee hintatiedot API:sta
		float getPrice(time_t tTime); // Kysytyn ajan hintatieto
		int savePrices(string file); // Tallentaa hintatiedot tekstitiedostoon, erotin ;
		
	private:
		time_t chTime(string jsDate);
		
		time_t latestTime = 0; // Viimeisen hintatiedon loppuaika
		int tZone; // Aikavyöhyke sis. kesäajan
		struct hPrice_t {
			float price; // Hinta
			time_t start; // Alkuaika
			time_t end; // Loppuaika
		} hourPrice;
		vector <hPrice_t> hourlyPrices;		

};