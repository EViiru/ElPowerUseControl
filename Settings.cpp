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
      
      size_t start, end, next = 0;
      
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
			
			if(parseSetLine(tietorivi))
				cout << "Tietorivin tulkinta ei onnistunut." << endl;
			
   		next = end;
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
	return raja;
}

int Settings::parseSetLine(string tRivi) { // Puretaan asetusrivi ja talletetaan muuttuneet asetukset
			size_t start = 0, end, pos;
   		
   		while (1) {   	
   			pos = tRivi.find_first_of("# \t", start); // Poistetaan ##, välilyönnit ja tabulaattorit
   			if (pos > tRivi.size()) {
   				break;
   			}
				tRivi.erase(pos, 1);
   			start = pos;  		
   		}
   		
   		start = 0;
   		while (1) {   	
   			pos = tRivi.find_first_of(",", start); // Vaihdetaan (desimaali)pilkku pisteeksi
   			if (pos > tRivi.size()) {
   				break;
   			}
				tRivi.replace(pos, 1, ".");
   			start = pos;  		
   		}
    					
			string nimi, luku;			
			float alaraja, ylaraja;			
			start = 0;

			end = tRivi.find(";", start);
			nimi = tRivi.substr(start, end - start);
			
			start = end + 1;
			end = tRivi.find(";", start);
			luku = tRivi.substr(start, end - start);
			
			if((alaraja = strToFloat(luku)) < -99998) {
				cout << "Alarajan tulkinta ei onnistunut." << endl;
				return -1;
			}	
 			
			start = end + 1;
			end = tRivi.find(";", start);
			luku = tRivi.substr(start, end - start);
			
			if((ylaraja = strToFloat(luku)) < -99998) {
				cout << "Ylärajan tulkinta ei onnistunut." << endl;
				return -1;
			}				
			
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

	return 0;
}


float Settings::strToFloat(string sLuku) { // Merkkijonon tarkastus ja muunto liukuluvuksi
	float luku = -99999;
	size_t pos, viim, start;
	int lkm;
	
	pos = 0;
   while (1) {   	
   	pos = sLuku.find_first_not_of("+-.0123456789", pos); // Poistetaan muut kuin numerot, +, - ja .
   	if (pos > sLuku.size()) {
   		break;
   	}
		sLuku.erase(pos, 1);
   }
   
   pos = sLuku.find_first_of("+-", 1); // (+) tai (-) vain ensimmäisenä merkkinä
   if (pos <= sLuku.size()) {
		cout << "Virhe liukulukumuunnoksessa." << endl;
		return luku;
   }
   
	start = 0;
	lkm = 0;
   while (1) {
   	pos = sLuku.find(".", start); // Max yksi (.)
  		if (pos > sLuku.size()) {
   		break;
   	}
   	lkm++;
   	start = pos + 1;
   }
   if(lkm > 1) {
		cout << "Virhe liukulukumuunnoksessa." << endl;
		return luku;   
   }
   
	luku = stof(sLuku);	
	return luku;
}





