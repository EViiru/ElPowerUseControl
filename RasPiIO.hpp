// Raspberryn GPIO

#include <iostream>
using std::cout;
using std::endl;
using std::ios;
#include <fstream>
using std::fstream;

using std::string;


/**
\brief Raspberry Pi:n lähtöjen ohjaus.
*
* Ohjataan Raspberry Pi:n lähtöjä "Linux-tyylisesti" tiedostojärjestelmän kautta:
* + otetaan ohjauspinnit käyttöön,
* + määritetään ne lähdöiksi ja
* + ohjataan niitä päälle/pois.
*/
class RasPiIO {

	public:
/**
* Rakentajassa varmistetaan, että lähdöt ovat pois päältä.
*/
		RasPiIO();
/**
* Purkajassa varmistetaan, että lähdöt ovat pois päältä.
*/
		~RasPiIO();
/**
* Kytkee ohjauslähdön päälle/pois.
* \param out 
* + true : päälle 
* + false : pois
* \return 0 : OK
* \return -1 : virhe
*/
		int setOut(bool out);
/**
* Kytkee vihreän LED:in päälle/pois.
* \param out 
* + true : päälle 
* + false : pois
* \return 0 : OK
* \return -1 : virhe
*/
		int setGreen(bool out);
/**
* Kytkee keltaisen LED:in päälle/pois.
* \param out 
* + true : päälle 
* + false : pois
* \return 0 : OK
* \return -1 : virhe
*/
		int setYellow(bool out);
/**
* Kytkee punaisen LED:in päälle/pois.
* \param out 
* + true : päälle 
* + false : pois
* \return 0 : OK
* \return -1 : virhe
*/
		int setRed(bool out);
/**
* Kytkee heartbeat-LED:in päälle/pois.
* \param out 
* + true : päälle 
* + false : pois
* \return 0 : OK
* \return -1 : virhe
*/
		int setHbeat(bool out);
/**
* Kytkee varaus-LED:in päälle/pois.
* \param out 
* + true : päälle 
* + false : pois
* \return 0 : OK
* \return -1 : virhe
*/
		int setRes(bool out);
		
	private:
		int setOutPin(bool out, string gp_pin); /**< Lähdön ohjaus */

		struct PinOut {
			bool inv_ = 1; /**< 1 : Alhaalla aktiivinen lähtö */
			string pin_ = "24"; /**< GPIO-numero */
		} pinOut_; /**< Ohjauslähtö */
			
		PinOut pinGreen_ = {1, "14"}; /**< LED, vihreä */
		PinOut pinYellow_ = {1, "15"}; /**< LED, keltainen */
		PinOut pinRed_ = {1, "18"}; /**< LED, punainen */
		PinOut pinHbeat_ = {1, "23"}; /**< LED, pulssi */
		PinOut pinRes_ = {1, "25"}; /**< Varalähtö */
		
		RasPiIO(RasPiIO const& src); /**< Kopiorakentaja (oletus) estetty */
		RasPiIO& operator=(RasPiIO const& src); /**< Sijoitus (oletus) estetty */

};