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

int Settings::updateSettings(string fileName) { // Haetaan asetukset tiedostosta
	// Haetaan asetusarvot tiedostosta
	ifstream f3_(fileName);
   string rajat_;
   ostringstream ss3_;
   
   if(f3_) {
      ss3_ << f3_.rdbuf();
      rajat_ = ss3_.str();
      
     size_t start_, end_, pos_;
            		
		start_ = 0;
   	while (1) {   	
   		pos_ = rajat_.find_first_of(" \t\n", start_); // Poistetaan välilyönnit, tabulaattorit ja rivinvaihdot
   		if (pos_ > rajat_.size()) {
   			break;
   		}
			rajat_.erase(pos_, 1);
   		start_ = pos_;  		
   	}

		start_ = 0;
		while (1) {
			string nimi_, sAla_, sYla_;			

			start_ = rajat_.find("\"", start_);
   		if (start_ > rajat_.size()) {
   			break;
   		}
   		start_ ++;
   		end_ = rajat_.find("\":", start_);
   		nimi_ = rajat_.substr(start_, end_ - start_);
   		
   		start_ = end_;
			start_ = rajat_.find("[", start_);
			start_ ++;
   		end_ = rajat_.find(",", start_);
   		sAla_ = rajat_.substr(start_, end_ - start_);
  		
    		start_ = end_ + 1;
   		end_ = rajat_.find("]", start_);
   		sYla_ = rajat_.substr(start_, end_ - start_);
 		
			OutputCntrl tempOut_;
			tempOut_.Name_ = nimi_;
			tempOut_.alar_ = strToFloat(sAla_);
			tempOut_.ylar_ = strToFloat(sYla_);		
						
			for (vector<OutputCntrl>::iterator it_ = outputLimits_.begin() ; it_ != outputLimits_.end(); ++it_) {
				outputCntrl_ = *it_;		
				if(outputCntrl_.Name_ == nimi_) {
					outputLimits_.erase(it_);
				}
			}
			outputLimits_.push_back(tempOut_);			
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





