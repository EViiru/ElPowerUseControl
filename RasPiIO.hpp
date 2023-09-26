// Raspberryn GPIO

#include <iostream>
#include <fstream>

using namespace std;

class RasPiIO {

	public:
		RasPiIO();
		~RasPiIO();
		int setOut(bool out); // Lähdön ohjaus
		
	private:
		// Lähdöt
		struct pinOut_t { // Ohjauslähtö
			bool inv = 1;
			string pin = "15";
		} pinOut;

};