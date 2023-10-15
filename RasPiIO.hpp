// Raspberryn GPIO

#include <iostream>
using std::cout;
using std::endl;
using std::ios;
#include <fstream>
using std::fstream;

using std::string;


class RasPiIO {

	public:
		RasPiIO();
		~RasPiIO();
		int setOut(bool out); // Ohjauslähtö
		int setGreen(bool out); // Vihreä LED
		int setYellow(bool out); // Keltainen LED
		int setRed(bool out); // Punainen LED
		int setHbeat(bool out); // Heartbeat LED
		int setRes(bool out); // Varaus LED
		
	private:
		int setOutPin(bool out, string gp_pin); // Lähdön ohjaus
		// Lähdöt
		struct pinOut_t { // Ohjauslähtö
			bool inv = 1;
			string pin = "24";
		} pinOut;
		struct pinGreen_t { // Ohjauslähtö
			bool inv = 1;
			string pin = "14";
		} pinGreen;
		struct pinYellow_t { // Ohjauslähtö
			bool inv = 1;
			string pin = "15";
		} pinYellow;
		struct pinRed_t { // Ohjauslähtö
			bool inv = 1;
			string pin = "18";
		} pinRed;
		struct pinHbeat_t { // Ohjauslähtö
			bool inv = 1;
			string pin = "23";
		} pinHbeat;
		struct pinRes_t { // Ohjauslähtö
			bool inv = 1;
			string pin = "25";
		} pinRes;

};