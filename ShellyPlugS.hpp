// ShellyPlugS ohjaus, REST API

#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::ostringstream;

using std::string;

/**
\brief ShellyPlugS-pistorasian ohjaus REST-API:lla.
*
* Pistorasialle määritetään IP-osoite, jonka kautta voidaan REST-API:lla:
* + lukea pistorasian tila ja
* + ohjata pistorasia päälle/pois.
*/
class ShellyPlugS {

	public:
/**
* Rakentajassa määritellään pistorasian IP-osoite ja varmistetaan, että pistorasia on pois päältä.
* \param baseAddrIP on pistorasian IP-osoite muodossa: "http://.../".
*/
		ShellyPlugS(string baseAddrIP);
/**
* Purkajassa varmistetaan, että pistorasia on pois päältä.
*/
		~ShellyPlugS();
/**
* Kytkee pistorasian päälle/pois.
* \param out 
* + true : päälle 
* + false : pois
* \return 0 : OK
* \return -1 : virhe
*/
		int setOut(bool out);
/**
* Lukee pistorasian tilan.
* \return 0 : pois
* \return 1 : päällä
* \return -1 : virhe
*/		
		int readOut();
		
	private:
		string baseAddrIP_; /**< Laitteen IP-osoite */
		
		ShellyPlugS(ShellyPlugS const& src); /**< Kopiorakentaja (oletus) estetty */
		ShellyPlugS& operator=(ShellyPlugS const& src); /**< Sijoitus (oletus) estetty */
		
};