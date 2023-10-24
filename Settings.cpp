#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include "Settings.hpp"
#endif

Settings::Settings() {
	cout << "Settings: Rakentaja" << endl;
	
	// Oletusarvot
	outputCntrl_.Name_ = "Output";
	outputCntrl_.alar_ = -9999;
	outputCntrl_.ylar_ = 2.5;
	outputLimits_.push_back(outputCntrl_);
	
	outputCntrl_.Name_ = "Green";
	outputCntrl_.alar_ = -9999;
	outputCntrl_.ylar_ = 5.0;
	outputLimits_.push_back(outputCntrl_);

	outputCntrl_.Name_ = "Yellow";
	outputCntrl_.alar_ = 2.5;
	outputCntrl_.ylar_ = 10.0;
	outputLimits_.push_back(outputCntrl_);

	outputCntrl_.Name_ = "Red";
	outputCntrl_.alar_ = 7.5;
	outputCntrl_.ylar_ = 9999;
	outputLimits_.push_back(outputCntrl_);
}

Settings::~Settings() {
	cout << "Settings: Purkaja" << endl;
}

int Settings::updateSettings(string fileName) { // Haetaan asetukset tiedostosta: ohjausrajat.txt
	// Haetaan asetusarvot tiedostosta
	ifstream f3_(fileName);
   string rajat_, tietorivi_;
   ostringstream ss3_;

   if(f3_) {
      ss3_ << f3_.rdbuf();
      rajat_ = ss3_.str();
      
      size_t start_, end_, next_ = 0;
      
   	while (1) {   	
   		start_ = rajat_.find("//", next_); // Poistetaan kommentit
   		if (start_ > rajat_.size()) {
   			break;
   		}
   		end_ = rajat_.find("\n", start_);
			rajat_.erase(start_, end_ - start_);
   		next_ = start_;  		
   	}
      
 		next_ = 0;
      while (1) {   	
   		start_ = rajat_.find("##", next_); // Haetaan tietorivi
   		if (start_ > rajat_.size()) {
   			break;
   		}
   		end_ = rajat_.find("\n", start_);
			tietorivi_ = rajat_.substr(start_, end_ - start_);
			
			if(parseSetLine(tietorivi_))
				cout << "Settings: Tietorivin tulkinta ei onnistunut." << endl;
			
   		next_ = end_;
   	}	
   	
   	for (vector<OutputCntrl>::iterator it_ = outputLimits_.begin() ; it_ != outputLimits_.end(); ++it_) {
			outputCntrl_ = *it_;
			cout << outputCntrl_.Name_ << " ala: " << outputCntrl_.alar_ << " ylä: " << outputCntrl_.ylar_ << endl;		
   	}
   }
   else {
   	cout << "Settings: Asetustiedoston luku epäonnistui: käytetään oletusarvoja." << endl;
   	return -1;  	
   }

	return 0;
}

float Settings::getLowLimit(string outName) { // Lähdön ohjauksen alaraja
	float raja_ = 9999;
	
	for (vector<OutputCntrl>::iterator it_ = outputLimits_.begin() ; it_ != outputLimits_.end(); ++it_) {
		outputCntrl_ = *it_;
		
		if(outputCntrl_.Name_ == outName) {
			raja_ = outputCntrl_.alar_;	
		}
	}
	return raja_;
}

float Settings::getHighLimit(string outName) { // Lähdön ohjauksen yläraja
	float raja_ = -9999;
	
	for (vector<OutputCntrl>::iterator it_ = outputLimits_.begin() ; it_ != outputLimits_.end(); ++it_) {
		outputCntrl_ = *it_;
		
		if(outputCntrl_.Name_ == outName) {
			raja_ = outputCntrl_.ylar_;	
		}
	}
	return raja_;
}

int Settings::parseSetLine(string tRivi) { // Puretaan asetusrivi ja talletetaan muuttuneet asetukset
			size_t start_ = 0, end_, pos_;
   		
   		while (1) {   	
   			pos_ = tRivi.find_first_of("# \t", start_); // Poistetaan ##, välilyönnit ja tabulaattorit
   			if (pos_ > tRivi.size()) {
   				break;
   			}
				tRivi.erase(pos_, 1);
   			start_ = pos_;  		
   		}
   		
   		start_ = 0;
   		while (1) {   	
   			pos_ = tRivi.find_first_of(",", start_); // Vaihdetaan (desimaali)pilkku pisteeksi
   			if (pos_ > tRivi.size()) {
   				break;
   			}
				tRivi.replace(pos_, 1, ".");
   			start_ = pos_;  		
   		}
    					
			string nimi_, luku_;			
			float alaraja_, ylaraja_;			
			start_ = 0;

			end_ = tRivi.find(";", start_);
			nimi_ = tRivi.substr(start_, end_ - start_);
			
			start_ = end_ + 1;
			end_ = tRivi.find(";", start_);
			luku_ = tRivi.substr(start_, end_ - start_);
			
			if((alaraja_ = strToFloat(luku_)) < -99998) {
				cout << "Settings: Alarajan tulkinta ei onnistunut." << endl;
				return -1;
			}	
 			
			start_ = end_ + 1;
			end_ = tRivi.find(";", start_);
			luku_ = tRivi.substr(start_, end_ - start_);
			
			if((ylaraja_ = strToFloat(luku_)) < -99998) {
				cout << "Settings: Ylärajan tulkinta ei onnistunut." << endl;
				return -1;
			}				
			
			OutputCntrl tempOut_;
			tempOut_.Name_ = nimi_;
			tempOut_.alar_ = alaraja_;
			tempOut_.ylar_ = ylaraja_;		
						
			for (vector<OutputCntrl>::iterator it_ = outputLimits_.begin() ; it_ != outputLimits_.end(); ++it_) {
				outputCntrl_ = *it_;		
				if(outputCntrl_.Name_ == nimi_) {
					outputLimits_.erase(it_);
					outputLimits_.push_back(tempOut_);
				}
			}

	return 0;
}


float Settings::strToFloat(string sLuku) { // Merkkijonon tarkastus ja muunto liukuluvuksi
	float luku_ = -99999;
	size_t pos_, viim_, start_;
	int lkm_;
	
	pos_ = 0;
   while (1) {   	
   	pos_ = sLuku.find_first_not_of("+-.0123456789", pos_); // Poistetaan muut kuin numerot, +, - ja .
   	if (pos_ > sLuku.size()) {
   		break;
   	}
		sLuku.erase(pos_, 1);
   }
   
   pos_ = sLuku.find_first_of("+-", 1); // (+) tai (-) vain ensimmäisenä merkkinä
   if (pos_ <= sLuku.size()) {
		cout << "Settings: Virhe liukulukumuunnoksessa." << endl;
		return luku_;
   }
   
	start_ = 0;
	lkm_ = 0;
   while (1) {
   	pos_ = sLuku.find(".", start_); // Max yksi (.)
  		if (pos_ > sLuku.size()) {
   		break;
   	}
   	lkm_++;
   	start_ = pos_ + 1;
   }
   if(lkm_ > 1) {
		cout << "Settings: Virhe liukulukumuunnoksessa." << endl;
		return luku_;   
   }
   
	luku_ = stof(sLuku);	
	return luku_;
}





