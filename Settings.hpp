// Asetusarvot

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>

using namespace std;

class Settings {

	public:
		Settings();
		~Settings();
		int updateSettings(string fileName); // Haetaan asetukset tiedostosta
		float getLowLimit(string outName); // Lähdön ohjauksen alaraja
		float getHighLimit(string outName); // Lähdön ohjauksen yläraja
		
	private:
		// Ohjausrajat
		struct OutputCntrl {
			string Name; // Lähdön nimi
			float alar; // Alaraja
			float ylar; // Yläraja
		} outputCntrl;
		vector <OutputCntrl> outputLimits;		
};
