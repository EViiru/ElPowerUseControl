// Asetusarvot

#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::ostringstream;
#include <vector>
using std::vector;

using std::string;


/**
\brief Ohjelman asetusarvot
*
* Hakee ohjelman asetusarvot tiedostosta.
*/
class Settings {

	public:
/**
* Rakentajassa määritellään oletusarvot asetuksille. Ne voidaan korvata tiedostosta luettavilla arvoilla.
*/
		Settings();
/**
* Purkajassa ei ole toimintoja.
*/
		~Settings();
/**
* Hakee asetusarvot tiedostosta.
* \param fileName : asetusarvotiedoston nimi 
* \return 0 : OK
* \return -1 : virhe
*/
		int updateSettings(string fileName);
/**
* Palauttaa asetuksen alarajan.
* \param outName : lähdön nimi 
* \return <9999 : asetuksen alaraja
* \return 9999 : virhe
*/
		float getLowLimit(string outName);
/**
* Palauttaa asetuksen ylärajan.
* \param outName : lähdön nimi 
* \return >-9999 : asetuksen yläraja
* \return -9999 : virhe
*/
		float getHighLimit(string outName);
		
	private:
		int parseSetLine(string tRivi); /**< Puretaan asetusrivi ja talletetaan muuttuneet asetukset */
		float strToFloat(string sLuku); /**< Merkkijonon tarkastus ja muunto liukuluvuksi */
		
		// Ohjausrajat
		struct OutputCntrl {
			string Name_; /**< Lähdön nimi */
			float alar_; /**< Alaraja */
			float ylar_; /**< Yläraja */
		} outputCntrl_; /**< Yhden lähdön tiedot */
		vector <OutputCntrl> outputLimits_; /**< Kaikkien lähtöjen tiedot */
				
		Settings(Settings const& src); /**< Kopiorakentaja (oletus) estetty */
		Settings& operator=(Settings const& src); /**< Sijoitus (oletus) estetty */

};
