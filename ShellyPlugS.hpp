// ShellyPlugS ohjaus, REST API

#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::ostringstream;

using std::string;


class ShellyPlugS {

	public:
		ShellyPlugS(string baseAddrIP);
		~ShellyPlugS();
		int setOut(bool out); // Kytkennän ohjaus
		int readOut(); // Kytkennän tila
		
	private:
		string baseAddrIP_; // Laitteen IP-osoite
		
		ShellyPlugS(ShellyPlugS const& src); // Kopiorakentaja (oletus) estetty
		ShellyPlugS& operator=(ShellyPlugS const& src); // Sijoitus (oletus) estetty
};