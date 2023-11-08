#ifndef CONTROL_HPP
#define CONTROL_HPP
#include "Control.hpp"
#endif

Control::Control() {
	cout << "Control: Rakentaja" << endl;
}

Control::~Control() {
	cout << "Control: Purkaja" << endl;
}

int Control::cntrlOn() { // Ohjaus käynnissä
	int exMin_ = 0, exOut_ = 0, cMin_, ret_;
	bool outPut_ = 0, heartBeat_ = 0;
	float elPrice_;
	time_t t_prev_ = 0;
	nextUpdate_ = 0;
	FILE *file_;
	SpotPrices sp_;
	RasPiIO raspi_;
	Settings outOn_;
	ShellyPlugS plug01_(SHELLYPLUGS_IP);
	
	ret_ = outOn_.updateSettings(FILENAME_SETTINGS); // Haetaan asetukset tiedostosta
	
	while(1) {
		time_t t_ = time(NULL);
		tm lTime_ = *localtime(&t_);
	
		if((t_ - t_prev_) > 5) { //Suoritetaan 5s välein
			t_prev_ = t_;
			system("touch ./temp/alive"); // Vahtikoiralle ruokaa
			system("touch ./temp/controlOn"); // Ohjaus käynnissä

			// Tilan vaihto -> stop		
			if(file_ = fopen ("stop","r")) {
				fclose(file_);
				system("rm stop");
				if(file_ = fopen ("./temp/outputs.json","r")) {
					fclose(file_);
					system("rm ./temp/outputs.json");
				}
				break;
			}
			
			if(file_ = fopen ("update","r")) { // Asetuksia on muutettu
				fclose(file_);
				system("rm update");
				ret_ = outOn_.updateSettings(FILENAME_SETTINGS); // Haetaan asetukset tiedostosta
			}
							
			raspi_.setHbeat(heartBeat_);
			heartBeat_ = !heartBeat_;
		}
		
		while( exMin_ != (cMin_ = lTime_.tm_min)) { // Suoritetaan kerran minuutissa
			
			if((time(NULL) - nextUpdate_) > 0) { // Päivitetään hinnat
				ret_ = sp_.updatePrices();
				if(!ret_) {
					nextUpdate_ = sp_.getLatestTime() - UPDATE_TIME; // Seuraava hintojen päivitysaika,
					srand(time(NULL));
					nextUpdate_ -= rand() % 3600; // johon lisätään vähän hajontaa
					sp_.savePrices(FILENAME_PRICES); // Tallennetaan hinnat tiedostoon
				}
			}
			
			elPrice_ = sp_.getPrice(time(NULL));
			
			// Ohjauslähtö
			int bitMask_ = 0x01;
			outPut_ = 0;												
			if((elPrice_ > outOn_.getLowLimit("Output")) && (elPrice_ <= outOn_.getHighLimit("Output")))
				outPut_ = 1;
			if(outPut_ != bool(exOut_ & bitMask_)) {
				outPut_ ? exOut_ |= bitMask_ : exOut_ &=  ~bitMask_;
				cout << "Time: " << asctime(&lTime_);
				cout << "Price: " << elPrice_ << " Out: " << outPut_ << endl;							
			}
			raspi_.setOut(outPut_);
			ret_ = plug01_.setOut(outPut_);
			
			// Vihreä LED
			bitMask_ = 0x02;
			outPut_ = 0;														
			if((elPrice_ > outOn_.getLowLimit("Green")) && (elPrice_ <= outOn_.getHighLimit("Green")))
				outPut_ = 1;
			if(outPut_ != bool(exOut_ & bitMask_)) {
				outPut_ ? exOut_ |= bitMask_ : exOut_ &= ~bitMask_;
				cout << "Time: " << asctime(&lTime_);
				cout << "Price: " << elPrice_ << " Green: " << outPut_ << endl;							
			}				
			raspi_.setGreen(outPut_);		
			
			// Keltainen LED
			bitMask_ = 0x04;
			outPut_ = 0;														
			if((elPrice_ > outOn_.getLowLimit("Yellow")) && (elPrice_ <= outOn_.getHighLimit("Yellow")))
				outPut_ = 1;
			if(outPut_ != bool(exOut_ & bitMask_)) {
				outPut_ ? exOut_ |= bitMask_ : exOut_ &= ~bitMask_;
				cout << "Time: " << asctime(&lTime_);
				cout << "Price: " << elPrice_ << " Yellow: " << outPut_ << endl;							
			}				
			raspi_.setYellow(outPut_);		

			// Punainen LED
			bitMask_ = 0x08;
			outPut_ = 0;														
			if((elPrice_ > outOn_.getLowLimit("Red")) && (elPrice_ <= outOn_.getHighLimit("Red")))
				outPut_ = 1;
			if(outPut_ != bool(exOut_ & bitMask_)) {
				outPut_ ? exOut_ |= bitMask_ : exOut_ &= ~bitMask_;
				cout << "Time: " << asctime(&lTime_);
				cout << "Price: " << elPrice_ << " Red: " << outPut_ << endl;							
			}				
			raspi_.setRed(outPut_);					

			// Lähtöjen tila tiedostoon
			fstream oFile_;
			oFile_.open("./temp/outputs.json", ios::out | ios::trunc);
			if (oFile_.is_open()){
				oFile_ << "{" << endl;
				oFile_ << "    \"Output\": ";
				bitMask_ = 0x01;
				bool(exOut_ & bitMask_) ? oFile_ << "true," : oFile_ << "false,";
				oFile_ << endl;
				oFile_ << "    \"Green\": ";
				bitMask_ = 0x02;
				bool(exOut_ & bitMask_) ? oFile_ << "true," : oFile_ << "false,";
				oFile_ << endl;
				oFile_ << "    \"Yellow\": ";
				bitMask_ = 0x04;
				bool(exOut_ & bitMask_) ? oFile_ << "true," : oFile_ << "false,";
				oFile_ << endl;
				oFile_ << "    \"Red\": ";
				bitMask_ = 0x08;
				bool(exOut_ & bitMask_) ? oFile_ << "true" : oFile_ << "false";
				oFile_ << endl << "}" << endl;
				
				oFile_.close();				
			}		
						
			exMin_ = cMin_;	
		}

	}
	return 0;
}

int Control::cntrlOff() { // Ohjaus pysähdyksissä
	time_t t_prev_ = 0;
	FILE *file_;

	while(1) {
		time_t t_ = time(NULL);
	
		if((t_ - t_prev_) > 5) { //Suoritetaan 5s välein
			t_prev_ = t_;
			system("touch ./temp/alive"); // Vahtikoiralle ruokaa
			if(file_ = fopen ("./temp/controlOn","r")) {
				fclose(file_);
				system("rm ./temp/controlOn"); // Ohjaus pysähdyksissä
			}

			// Tilan vaihto -> start					
			if(file_ = fopen ("start","r")) {
				fclose(file_);
				system("rm start");
				break;
			}	
		}
		
	}
	return 0;	
}


