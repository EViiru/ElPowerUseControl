#ifndef SETTINGS
#define SETTINGS
#include "Settings.hpp"
#endif

Settings::Settings() {
	cout << "Settings: Rakentaja" << endl;
	
	// Oletusarvot
	outputCntrl.Name = "Output";
	outputCntrl.alar = -9999;
	outputCntrl.ylar = 2.5;
	outputLimits.push_back(outputCntrl);
	
	outputCntrl.Name = "Green";
	outputCntrl.alar = -9999;
	outputCntrl.ylar = 5.0;
	outputLimits.push_back(outputCntrl);

	outputCntrl.Name = "Yellow";
	outputCntrl.alar = 2.5;
	outputCntrl.ylar = 10.0;
	outputLimits.push_back(outputCntrl);

	outputCntrl.Name = "Red";
	outputCntrl.alar = 7.5;
	outputCntrl.ylar = 9999;
	outputLimits.push_back(outputCntrl);
}

Settings::~Settings() {
	cout << "Settings: Purkaja" << endl;
}

int Settings::updateSettings(string fileName) { // Haetaan asetukset tiedostosta: ohjausrajat.txt
	// Haetaan asetusarvot tiedostosta
	ifstream f3(fileName); // Taking file as inputstream
   string rajat, tietorivi;
   ostringstream ss3;

   if(f3) {
      ss3 << f3.rdbuf(); // Reading data
      rajat = ss3.str();
      
      size_t pos, start, end, next = 0;
      
   	while (1) {   	
   		start = rajat.find("//", next); // Poistetaan kommentit
   		if (start > rajat.size()) {
   			break;
   		}
   		end = rajat.find("\n", start);
			rajat.erase(start, end - start);
   		next = start;  		
   	}
      
 		next = 0;
      while (1) {   	
   		start = rajat.find("##", next); // Haetaan tietorivi
   		if (start > rajat.size()) {
   			break;
   		}
   		end = rajat.find("\n", start);
			tietorivi = rajat.substr(start, end - start);
   		next = end;
   		
   		start = 0;
   		while (1) {   	
   			pos = tietorivi.find_first_of("# \t", start); // Poistetaan ##, välilyönnit ja tabulaattorit
   			if (pos > tietorivi.size()) {
   				break;
   			}
				tietorivi.erase(pos, 1);
   			start = pos;  		
   		}
   		
   		start = 0;
   		while (1) {   	
   			pos = tietorivi.find_first_of(",", start); // Vaihdetaan (desimaali)pilkku pisteeksi
   			if (pos > tietorivi.size()) {
   				break;
   			}
				tietorivi.replace(pos, 1, ".");
   			start = pos;  		
   		}
    					
			string nimi, luku;			
			float alaraja, ylaraja;			
			start = 0;

			end = tietorivi.find(";", start);
			nimi = tietorivi.substr(start, end - start);
			
			start = end + 1;
			end = tietorivi.find(";", start);
			luku = tietorivi.substr(start, end - start);
			
   		pos = 0;
   		while (1) {   	
   			pos = luku.find_first_not_of("+-.0123456789", pos); // Poistetaan muut kuin numerot, +, - ja .
   			if (pos > tietorivi.size()) {
   				break;
   			}
				luku.erase(pos, 1);
   		}
			alaraja = stof(luku);
 			
			start = end + 1;
			end = tietorivi.find(";", start);
			luku = tietorivi.substr(start, end - start);

   		pos = 0;
   		while (1) {   	
   			pos = luku.find_first_not_of("+-.0123456789", pos); // Poistetaan muut kuin numerot, +, - ja .
   			if (pos > tietorivi.size()) {
   				break;
   			}
				luku.erase(pos, 1);
   		}
			ylaraja = stof(luku);
			
			OutputCntrl tempOut;
			tempOut.Name = nimi;
			tempOut.alar = alaraja;
			tempOut.ylar = ylaraja;		
						
			for (vector<OutputCntrl>::iterator it = outputLimits.begin() ; it != outputLimits.end(); ++it) {
				outputCntrl = *it;		
				if(outputCntrl.Name == nimi) {
					outputLimits.erase(it);
					outputLimits.push_back(tempOut);
				}
			}
   	}
   	for (vector<OutputCntrl>::iterator it = outputLimits.begin() ; it != outputLimits.end(); ++it) {
			outputCntrl = *it;
			cout << "Nimi: " << outputCntrl.Name << " ala: " << outputCntrl.alar << " ylä: " << outputCntrl.ylar << endl;		
   	}
   }
   else {
   	cout << "Tiedoston ohjausrajat.txt luku epäonnistui: käytetään oletusarvoja." << endl;  	
   }

	return 0;
}

float Settings::getLowLimit(string outName) { // Lähdön ohjauksen alaraja
	float raja = 9999;
	
	for (vector<OutputCntrl>::iterator it = outputLimits.begin() ; it != outputLimits.end(); ++it) {
		outputCntrl = *it;
		
		if(outputCntrl.Name == outName) {
			raja = outputCntrl.alar;	
		}
	}
	cout << outName << ", ala: " << raja << endl;
	return raja;
}

float Settings::getHighLimit(string outName) { // Lähdön ohjauksen yläraja
	float raja = -9999;
	
	for (vector<OutputCntrl>::iterator it = outputLimits.begin() ; it != outputLimits.end(); ++it) {
		outputCntrl = *it;
		
		if(outputCntrl.Name == outName) {
			raja = outputCntrl.ylar;	
		}
	}
	cout << outName << ", ylä: " << raja << endl;
	return raja;
}



