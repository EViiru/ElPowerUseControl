// Sähkön hintatiedot

#include <iostream>
using std::cout;
using std::endl;
using std::ios;
#include <fstream>
using std::ifstream;
using std::fstream;
#include <sstream>
using std::ostringstream;
#include <vector>
using std::vector;

using std::string;
using std::to_string;

/**
\brief Sähkön hintatiedot
*
* Hakee pörssisähkön hintatiedot verkosta (https://api.porssisahko.net) ja tallentaa ne levylle historiatiedoksi.
*/
class SpotPrices {

	public:
/**
* Rakentajassa ei ole toimintoja.
*/
		SpotPrices();
/**
* Purkajassa ei ole toimintoja.
*/
		~SpotPrices();
/**
* Hakee pörssisähkön hintatiedot verkosta.
* \return 0 : OK
* \return -1 : virhe
*/
		int updatePrices();
/**
* Palauttaa kysytyn ajan hintatiedon.
* \param tTime : Aika, jonka hintatieto halutaan. 
* \return <99999 : Kysytyn ajan hintatieto.
* \return 99999 : Virhe, hintaa ei löytynyt.
*/
		float getPrice(time_t tTime);
/**
* Tallentaa hintatiedot tekstitiedostoon, kenttäerotin: ";"
* \param file : Tiedostonimen vakio-osa. Tiedostonimeen lisätään viimeisen hintatiedon aikaleima. 
* \return 0 : OK
* \return -1 : virhe
*/
		int savePrices(string file);
/**
* Palauttaa viimeisen hintatiedon loppuajan.
* \return >0 : Viimeisen hintatiedon loppuaika.
* \return 0 : Virhe, aikaa ei löytynyt.
*/
		time_t getLatestTime() const; 
		
	private:
		time_t chTime(string jsDate); /**< Muuttaa json-aikatiedon unix/linux-ajaksi. */
		
		time_t latestTime_ = 0; /**< Viimeisen hintatiedon loppuaika */
		int tZone_; /**< Aikavyöhyke sis. kesäajan */
		struct HPrice {
			float price_; /**< Hinta */
			time_t start_; /**< Alkuaika */
			time_t end_; /**< Loppuaika */
		} hourPrice_; /**< Sähkön tuntihinta (yksi tunti) */
		vector <HPrice> hourlyPrices_; /**< Sähkön viimeiset tuntihinnat */
		
		SpotPrices(SpotPrices const& src); /**< Kopiorakentaja (oletus) estetty */
		SpotPrices& operator=(SpotPrices const& src); /**< Sijoitus (oletus) estetty */

};