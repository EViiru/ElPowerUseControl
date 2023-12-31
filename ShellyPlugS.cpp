#ifndef SHELLYPLUGS_HPP
#define SHELLYPLUGS_HPP
#include "ShellyPlugS.hpp"
#endif

//#define SHELLYPLUGS // ShellyPlugS on käytössä

ShellyPlugS::ShellyPlugS(string baseAddrIP) {
	baseAddrIP_ = baseAddrIP; // Laitteen IP-osoite
	cout << "ShellyPlugS/" << baseAddrIP_ << ": Rakentaja" << endl;
	
	setOut(false); // Nollataan lähtö

}

ShellyPlugS::~ShellyPlugS() {
	cout << "ShellyPlugS/" << baseAddrIP_ << ": Purkaja" << endl;
	
	setOut(false); // Nollataan lähtö

}

int ShellyPlugS::setOut(bool out) { // Kytkennän ohjaus
 
#ifdef SHELLYPLUGS
	int currStInt_;
	if ((currStInt_ = readOut()) < 0) {
		return -1;	
	}
	
	bool currSt_ = bool(currStInt_);
		
	if (out ^ currSt_) { // Tarvitseeko vaihtaa?
		string endpCall_ = "wget --timeout=5 --tries=1 "; // Muodostetaan endpoint-kutsu (REST API)
		endpCall_.append(baseAddrIP_);
		endpCall_.append("relay/0");
		if (out) { 		
			endpCall_.append("?turn=on");
		}
		else {
			endpCall_.append("?turn=off");
		}
		endpCall_.append(" -O ./temp/StatusRel0.json -o ./temp/QueryStatus.txt");
		system(endpCall_.data()); // Asetetaan lähdön tila

		time_t now_ = time(NULL);		
		cout << "Time: " << asctime(localtime(&now_)); 
   	cout << "ShellyPlugS/" << baseAddrIP_ << ": " << out << endl;


	}
#endif
	 	
 	return 0;
}

int ShellyPlugS::readOut() { // Kytkennän tila

#ifdef SHELLYPLUGS
	string endpCall_ = "wget --timeout=5 --tries=1 "; // Muodostetaan endpoint-kutsu (REST API)
	endpCall_.append(baseAddrIP_);
	endpCall_.append("relay/0");
	endpCall_.append(" -O ./temp/StatusRel0.json -o ./temp/QueryStatus.txt");

	system(endpCall_.data()); // Haetaan lähdön tila
	
	ostringstream ss1_, ss2_;
	
	// Tarkistetaan api-haun tilatieto	
	ifstream f1_("./temp/QueryStatus.txt"); // Taking file as inputstream
   string response_;
   if(f1_) {
      ss1_ << f1_.rdbuf(); // Reading data
      response_ = ss1_.str();
   }
   else {
		time_t now_ = time(NULL);
		cout << "Time: " << asctime(localtime(&now_)); 
   	cout << "ShellyPlugS/" << baseAddrIP_ << ": Tiedoston luku epäonnistui, status" << endl;
   	return -1;
   }
	if (response_.find("200 OK") != -1) { // Onnistuiko haku?
	}
	else {
		time_t now_ = time(NULL);
		cout << "Time: " << asctime(localtime(&now_)); 
		cout << "ShellyPlugS/" << baseAddrIP_ << ": Haku epäonnistui" << endl;
		return -1;
	}

	// Haetaan lähdön tila
	ifstream f2_("./temp/StatusRel0.json"); // Taking file as inputstream
   string status_;
   if(f2_) {
      ss2_ << f2_.rdbuf(); // Reading data
      status_ = ss2_.str();
   }
   else {
		time_t now_ = time(NULL);
		cout << "Time: " << asctime(localtime(&now_)); 
   	cout << "ShellyPlugS/" << baseAddrIP_ << ": Tiedoston luku epäonnistui, data" << endl;
   	return -1;   	
   }

   size_t start_, end_;
   start_ = status_.find("\"ison\":", 0); // Parametri "ison"
   if (start_ > status_.size()) {
		time_t now_ = time(NULL);
		cout << "Time: " << asctime(localtime(&now_)); 
		cout << "ShellyPlugS/" << baseAddrIP_ << ": Parametrin nimen haku epäonnistui" << endl;
		return -1;
   }
   end_ = status_.find(",", start_); // Parametrin loppu
	string parIson_ = status_.substr(start_, end_ - start_);
		
	if (parIson_.find("true", 0) <= parIson_.size()) {
		return 1;	
	}
	else if (parIson_.find("false", 0) <= parIson_.size()) {
		return 0;	
	}
#endif
	
	cout << "ShellyPlugS/" << baseAddrIP_ << ": Parametrin arvon haku epäonnistui" << endl;
	return -1;

}
  


