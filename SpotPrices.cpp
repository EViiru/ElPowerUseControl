#ifndef SPOTPRICES_HPP
#define SPOTPRICES_HPP
#include "SpotPrices.hpp"
#endif


SpotPrices::SpotPrices() {
	cout << "SpotPrices: Rakentaja" << endl;	
}

SpotPrices::~SpotPrices() {
	cout << "SpotPrices: Purkaja" << endl;
}

int SpotPrices::updatePrices() { // Hakee hintatiedot API:sta: Pörssisähkö.net
	
	ostringstream ss1_, ss2_;
	
	// Aikavyöhyke
	time_t now_ = time(NULL);
	tm locTime_ = *localtime(&now_);
	tm gmtTime_ = *gmtime(&now_);
	cout << "Time: " << asctime(&locTime_);
	cout << "SpotPrices: updatePrices" << endl;
	tZone_ = locTime_.tm_hour - gmtTime_.tm_hour; // -23...+23
	if(tZone_ <= -12 || tZone_ > 12) {
		tZone_ = tZone_ - 24 * (tZone_ / abs(tZone_)); // -11...+12	
	}
	
	// Haetaan sähkön hinnat tiedostoon, 48h
	system("wget --timeout=5 --tries=1 https://api.porssisahko.net/v1/latest-prices.json -O ./temp/SpotPrices.json -o ./temp/Success.txt" ); 
	
	// Tarkistetaan api-haun tilatieto	
	ifstream f1_("./temp/Success.txt");
   string response_;
   if(f1_) {
      ss1_ << f1_.rdbuf();
      response_ = ss1_.str();
   }
   else {
   	cout << "SpotPrices: Tiedoston luku epäonnistui, status" << endl;
   	return -1;
   }
	if (response_.find("200 OK") != -1) { // Onnistuiko haku?
		cout << "SpotPrices: Haku onnistui" << endl;
	}
	else {
		cout << "SpotPrices: Haku epäonnistui" << endl;
		return -1;
	}
	
	// Haetaan hintatiedot taulukkoon (vector))
	ifstream f2_("./temp/SpotPrices.json");
   string prices_json_;
   if(f2_) {
      ss2_ << f2_.rdbuf();
      prices_json_ = ss2_.str();
   }
   else {
   	cout << "SpotPrices: Tiedoston luku epäonnistui, data" << endl;
   	return -1;   	
   }

   size_t pos_, start_, end_, next_ = 0;
   hourlyPrices_.clear(); // Tyhjennetään vanhat tiedot
   while (1) {   	
   	pos_ = prices_json_.find("{\"price\":", next_); // Hintarivin alku
   	if (pos_ > prices_json_.size()) {
   		break;
   	}

   	// Hinta
   	start_ = pos_ + 9;
   	end_ = prices_json_.find(",", start_);
   	hourPrice_.price_ = stof(prices_json_.substr(start_, end_ - start_)); // Hinta
 
 		// Alkuaika
   	start_ = prices_json_.find("startDate", pos_);
   	start_ += 12;
   	end_ = prices_json_.find(",", start_) -1;
   	string startTime_ = prices_json_.substr(start_, end_ - start_); // Alkuaika
   	hourPrice_.start_ = chTime(startTime_);   	

 		// Loppuaika
   	start_ = prices_json_.find("endDate", pos_);
   	start_ += 10;
   	end_ = prices_json_.find("}", start_) -1;
   	string endTime_ = prices_json_.substr(start_, end_ - start_); // Loppuaika
   	hourPrice_.end_ = chTime(endTime_);
   	
   	if(hourPrice_.end_ > latestTime_) // Tietojen loppuaika
   		latestTime_ = hourPrice_.end_;   	   	
  	
   	// Tiedot tuntihintataulukkoon (vector)
   	hourlyPrices_.push_back(hourPrice_);
   			  	
   	next_ = pos_ + 1;
   }	
   	
	return 0;
}

float SpotPrices::getPrice(time_t tTime) { // Kysytyn ajan hintatieto
	
	float elPrice_ = 99999; // Hintaa ei löytynyt
	
	for (vector<HPrice>::iterator it_ = hourlyPrices_.begin() ; it_ != hourlyPrices_.end(); ++it_) {
		hourPrice_ = *it_;
		if(hourPrice_.start_ <= tTime && hourPrice_.end_ > tTime)
			elPrice_ = hourPrice_.price_;		
	}

	return elPrice_;
}

int SpotPrices::savePrices(string file) { // Tallentaa hintatiedot tiedostoon
	cout << "SpotPrices: savePrices" << endl;
	
	// Tiedoston sisällön aikaleima tiedostonimeen
	tm latestTm_ = *localtime(&latestTime_); // Paikallinen aika
	unsigned long int latestLInt_ = latestTm_.tm_year - 100;
	latestLInt_ = 100 * latestLInt_ + latestTm_.tm_mon + 1;
	latestLInt_ = 100 * latestLInt_ + latestTm_.tm_mday;
	latestLInt_ = 100 * latestLInt_ + latestTm_.tm_hour;
	latestLInt_ = 100 * latestLInt_ + latestTm_.tm_min;	
	string extraDef_ = to_string(latestLInt_);
	
	size_t pos_;
	if(pos_ = file.find(".", 2))
		file.replace(pos_, 0, extraDef_);
		
	fstream oFile_;
	oFile_.open(file, ios::out | ios::trunc);
	if (oFile_.is_open()){		
		for (vector<HPrice>::iterator it_ = hourlyPrices_.begin() ; it_ != hourlyPrices_.end(); ++it_) {
			hourPrice_ = *it_;
			tm startTime_, endTime_;
			string oneLine_;
			size_t pos_;
				
			startTime_ = *localtime(&hourPrice_.start_); // Paikallinen aika
			endTime_ = *localtime(&hourPrice_.end_); // Paikallinen aika
		
			oneLine_ = to_string(hourPrice_.price_);
			pos_ = 0;
			if(pos_ = oneLine_.find("."))
				oneLine_.replace(pos_, 1, ",");
			oneLine_ += ";";

			if(startTime_.tm_mday < 10)
				oneLine_ += "0"; 
			oneLine_ += to_string(startTime_.tm_mday);
			oneLine_ += ".";
			if(startTime_.tm_mon + 1 < 10)
				oneLine_ += "0"; 
			oneLine_ += to_string(startTime_.tm_mon + 1);	
			oneLine_ += ".";
			oneLine_ += to_string(startTime_.tm_year + 1900);
			oneLine_ += ";";

			if(startTime_.tm_hour < 10)
				oneLine_ += "0"; 
			oneLine_ += to_string(startTime_.tm_hour);
			oneLine_ += ":";
			if(startTime_.tm_min < 10)
				oneLine_ += "0"; 
			oneLine_ += to_string(startTime_.tm_min);
			oneLine_ += ":";			
			if(startTime_.tm_sec < 10)
				oneLine_ += "0"; 
			oneLine_ += to_string(startTime_.tm_sec);	
			oneLine_ += ";";

			if(endTime_.tm_mday < 10)
				oneLine_ += "0"; 
			oneLine_ += to_string(endTime_.tm_mday);
			oneLine_ += ".";
			if(endTime_.tm_mon + 1 < 10)
				oneLine_ += "0"; 
			oneLine_ += to_string(endTime_.tm_mon + 1);	
			oneLine_ += ".";
			oneLine_ += to_string(endTime_.tm_year + 1900);
			oneLine_ += ";";

			if(endTime_.tm_hour < 10)
				oneLine_ += "0"; 
			oneLine_ += to_string(endTime_.tm_hour);
			oneLine_ += ":";
			if(endTime_.tm_min < 10)
				oneLine_ += "0"; 
			oneLine_ += to_string(endTime_.tm_min);
			oneLine_ += ":";			
			if(endTime_.tm_sec < 10)
				oneLine_ += "0"; 
			oneLine_ += to_string(endTime_.tm_sec);	

			oFile_ << oneLine_ << endl;
 	  }
	  oFile_.close();
	}
	else {
		cout << "SpotPrices: Tiedoston avausvirhe" << endl;
		return -1;	
	}
 
	return 0;
}

time_t SpotPrices::getLatestTime() const { // Viimeisen hintatiedon loppuaika
	return latestTime_;
} 
		
time_t SpotPrices::chTime(string jsDate) {
	time_t now_ = time(NULL);
	tm t_ = *localtime(&now_); // Haetaan kesä-/talviaika      
	time_t retTime_;

	t_.tm_year = stoi(jsDate.substr(0, 4)) - 1900;
	t_.tm_mon = stoi(jsDate.substr(5, 2)) - 1;
	t_.tm_mday = stoi(jsDate.substr(8, 2));
	t_.tm_hour = stoi(jsDate.substr(11, 2));
	t_.tm_min = 0;
	t_.tm_sec = 0;

	retTime_ = mktime(&t_) + tZone_ * 3600; // UTC ajaksi

	return retTime_;

}



