// ShellyPlugS ohjaus, REST API

#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;

class ShellyPlugS {

	public:
		ShellyPlugS(string baseAddrIP);
		~ShellyPlugS();
		int setOut(bool out); // Kytkennän ohjaus
		int readOut(); // Kytkennän tila
		
	private:
		string baseAddrIP_; // Laitteen IP-osoite
};