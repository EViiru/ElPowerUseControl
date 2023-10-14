
#ifndef SPOTPRICES
#define SPOTPRICES
#include "SpotPrices.hpp"
#endif


SpotPrices::SpotPrices() {
	cout << "SpotPrices: Rakentaja" << endl;	
}

SpotPrices::~SpotPrices() {
	cout << "SpotPrices: Purkaja" << endl;
}

int SpotPrices::updatePrices() { // Hakee hintatiedot API:sta: Pörssisähkö.net
	cout << "SpotPrices: updatePrices" << endl;
	
	ostringstream ss1, ss2;
	
	// Aikavyöhyke
	time_t now = time(NULL);
	tm locTime = *localtime(&now);
	tm gmtTime = *gmtime(&now);
	cout << "Loc: " << asctime(&locTime);
	cout << "Gmt: " << asctime(&gmtTime);
	tZone = locTime.tm_hour - gmtTime.tm_hour; // -23...+23
	if(tZone <= -12 || tZone > 12) {
		tZone = tZone - 24 * (tZone / abs(tZone)); // -11...+12	
	}
	cout << "TZ: " << tZone << endl;
	
	system("wget https://api.porssisahko.net/v1/latest-prices.json -O ./temp/SpotPrices.json -o ./temp/Success.txt" ); // Haetaan sähkön hinnat tiedostoon, 48h
	
	// Tarkistetaan api-haun tilatieto	
	ifstream f1("./temp/Success.txt"); // Taking file as inputstream
   string response;
   if(f1) {
      ss1 << f1.rdbuf(); // Reading data
      response = ss1.str();
   }
   else {
   	cout << "Tiedoston luku epäonnistui" << endl;
   	return -1;
   }
	if (response.find("200 OK") != -1) { // Onnistuiko haku?
		cout << "Haku onnistui" << endl;
	}
	else {
		cout << "Haku epäonnistui" << endl;
		return -1;
	}
	
	// Haetaan hintatiedot taulukkoon (vector))
	ifstream f2("./temp/SpotPrices.json"); // Taking file as inputstream
   string prices_json;
   if(f2) {
      ss2 << f2.rdbuf(); // Reading data
      prices_json = ss2.str();
   }
   else {
   	cout << "Tiedoston luku epäonnistui" << endl;
   	return -1;   	
   }

   size_t pos, start, end, next = 0;
   hourlyPrices.clear(); // Tyhjennetään vanhat tiedot
   while (1) {   	
   	pos = prices_json.find("{\"price\":", next); // Hintarivin alku
   	if (pos > prices_json.size()) {
   		break;
   	}

   	// Hinta
   	start = pos + 9;
   	end = prices_json.find(",", start);
   	hourPrice.price = stof(prices_json.substr(start, end - start)); // Hinta
 
 		// Alkuaika
   	start = prices_json.find("startDate", pos);
   	start += 12;
   	end = prices_json.find(",", start) -1;
   	string startTime = prices_json.substr(start, end - start); // Alkuaika
   	hourPrice.start = chTime(startTime);   	

 		// Loppuaika
   	start = prices_json.find("endDate", pos);
   	start += 10;
   	end = prices_json.find("}", start) -1;
   	string endTime = prices_json.substr(start, end - start); // Loppuaika
   	hourPrice.end = chTime(endTime);
   	
   	if(hourPrice.end > latestTime) // Tietojen loppuaika
   		latestTime = hourPrice.end;   	   	
  	
   	// Tiedot tuntihintataulukkoon (vector)
   	hourlyPrices.push_back(hourPrice);
   			  	
   	next = pos + 1;
   }	
   	
	return 0;
}

float SpotPrices::getPrice(time_t tTime) { // Kysytyn ajan hintatieto
//	cout << "SpotPrices: getPrice" << endl;
	
	float elPrice = 99999; // Hintaa ei löytynyt
	
	for (vector<hPrice_t>::iterator it = hourlyPrices.begin() ; it != hourlyPrices.end(); ++it) {
		hourPrice = *it;

/*		cout << hourPrice.price << " " \
			<< hourPrice.start << " " \
			<< hourPrice.end << endl;
*/
		if(hourPrice.start <= tTime && hourPrice.end > tTime)
			elPrice = hourPrice.price;		
	}

	return elPrice;
}

int SpotPrices::savePrices(string file) { // Tallentaa hintatiedot tiedostoon
	cout << "SpotPrices: savePrices" << endl;
	
	// Tiedoston sisällön aikaleima tiedostonimeen
	tm latestTm = *localtime(&latestTime); // Paikallinen aika
	unsigned long int latestLInt = latestTm.tm_year - 100;
	latestLInt = 100 * latestLInt + latestTm.tm_mon + 1;
	latestLInt = 100 * latestLInt + latestTm.tm_mday;
	latestLInt = 100 * latestLInt + latestTm.tm_hour;
	latestLInt = 100 * latestLInt + latestTm.tm_min;	
	string extraDef = to_string(latestLInt);
	
	size_t pos;
	if(pos = file.find("."))
		file.replace(pos, 0, extraDef);
		
	fstream oFile;
	oFile.open(file, ios::out | ios::trunc);
	if (oFile.is_open()){		
		for (vector<hPrice_t>::iterator it = hourlyPrices.begin() ; it != hourlyPrices.end(); ++it) {
			hourPrice = *it;
			tm startTime, endTime;
			string oneLine;
			size_t pos;
				
			startTime = *localtime(&hourPrice.start); // Paikallinen aika
			endTime = *localtime(&hourPrice.end); // Paikallinen aika
		
			oneLine = to_string(hourPrice.price);
			pos = 0;
			if(pos = oneLine.find("."))
				oneLine.replace(pos, 1, ",");
			oneLine += ";";

			if(startTime.tm_mday < 10)
				oneLine += "0"; 
			oneLine += to_string(startTime.tm_mday);
			oneLine += ".";
			if(startTime.tm_mon + 1 < 10)
				oneLine += "0"; 
			oneLine += to_string(startTime.tm_mon + 1);	
			oneLine += ".";
			oneLine += to_string(startTime.tm_year + 1900);
			oneLine += ";";

			if(startTime.tm_hour < 10)
				oneLine += "0"; 
			oneLine += to_string(startTime.tm_hour);
			oneLine += ":";
			if(startTime.tm_min < 10)
				oneLine += "0"; 
			oneLine += to_string(startTime.tm_min);
			oneLine += ":";			
			if(startTime.tm_sec < 10)
				oneLine += "0"; 
			oneLine += to_string(startTime.tm_sec);	
			oneLine += ";";

			if(endTime.tm_mday < 10)
				oneLine += "0"; 
			oneLine += to_string(endTime.tm_mday);
			oneLine += ".";
			if(endTime.tm_mon + 1 < 10)
				oneLine += "0"; 
			oneLine += to_string(endTime.tm_mon + 1);	
			oneLine += ".";
			oneLine += to_string(endTime.tm_year + 1900);
			oneLine += ";";

			if(endTime.tm_hour < 10)
				oneLine += "0"; 
			oneLine += to_string(endTime.tm_hour);
			oneLine += ":";
			if(endTime.tm_min < 10)
				oneLine += "0"; 
			oneLine += to_string(endTime.tm_min);
			oneLine += ":";			
			if(endTime.tm_sec < 10)
				oneLine += "0"; 
			oneLine += to_string(endTime.tm_sec);	

			oFile << oneLine << endl;
 	  }
	  oFile.close();
	}
	else {
		cout << "File open error" << endl;
		return -1;	
	}
 
	return 0;
}
		
time_t SpotPrices::chTime(string jsDate) {
	time_t now = time(NULL);
	tm t = *localtime(&now); // Haetaan kesä-/talviaika      
	time_t retTime;

//	cout << "In json: " << jsDate << endl;

	t.tm_year = stoi(jsDate.substr(0, 4)) - 1900;
//		cout << "year: " << t.tm_year << " ";
	t.tm_mon = stoi(jsDate.substr(5, 2)) - 1;
//		cout << "mon: " << t.tm_mon << " ";
	t.tm_mday = stoi(jsDate.substr(8, 2));
//		cout << "day: " << t.tm_mday << " ";
	t.tm_hour = stoi(jsDate.substr(11, 2));
//		cout << "hour: " << t.tm_hour << endl;
	t.tm_min = 0;
	t.tm_sec = 0;
//	cout << "Dst: " << t.tm_isdst << endl;

	retTime = mktime(&t) + tZone * 3600; // UTC ajaksi

//	cout << "Out: " << asctime(localtime(&retTime)) << endl;

	return retTime;

}



