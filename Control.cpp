#ifndef CONTROL
#define CONTROL
#include "Control.hpp"
#endif

Control::Control() {
	cout << "Control: Rakentaja" << endl;
}

Control::~Control() {
	cout << "Control: Purkaja" << endl;
}

int Control::cntrlOn() { // Ohjaus käynnissä
	int exMin = 0, cMin, ret;
	bool outPut = 0, heartBeat = 0;
	float elPrice;
	time_t t_prev = 0;
	lastUpdate = 0;
	FILE *file;
	SpotPrices sp;
	RasPiIO raspi;
	Settings outOn;
	
	ret = outOn.updateSettings("ohjausrajat.txt"); // Haetaan asetukset tiedostosta
	
	while(1) {
		time_t t = time(NULL);
		tm *lTime = localtime(&t);
	
		if((t - t_prev) > 5) { //Suoritetaan 5s välein
			t_prev = t;
			system("touch ./temp/alive"); // Vahtikoiralle ruokaa

			// Tilan vaihto -> stop		
			if(file = fopen ("stop","r")) {
				fclose(file);
				system("rm stop");
				system("rm ./temp/*");
				break;
			}	
			
			raspi.setHbeat(heartBeat);
			heartBeat = !heartBeat;
		}
		
		while( exMin != (cMin = lTime->tm_min)) { // Suoritetaan kerran minuutissa
			cout << "Control: cntrl " << cMin << endl;
			
			if((time(NULL) - lastUpdate) > updateTime) { // Päivitetään hinnat
				ret = sp.updatePrices();
				if(!ret) {
					sp.savePrices("ElPrices_.csv"); // Tallennetaan hinnat tiedostoon
					lastUpdate = time(NULL);
				}
			}
			
			elPrice = sp.getPrice(time(NULL));
			
			// Ohjauslähtö
			outPut = 0;												
			if((elPrice > outOn.getLowLimit("Output")) && (elPrice <= outOn.getHighLimit("Output")))
				outPut = 1;
			cout << "Price: " << elPrice << " Out: " << outPut << endl;			
			raspi.setOut(outPut);
			
			// Vihreä LED
			outPut = 0;														
			if((elPrice > outOn.getLowLimit("Green")) && (elPrice <= outOn.getHighLimit("Green")))
				outPut = 1;
			raspi.setGreen(outPut);		
			
			// Keltainen LED
			outPut = 0;														
			if((elPrice > outOn.getLowLimit("Yellow")) && (elPrice <= outOn.getHighLimit("Yellow")))
				outPut = 1;
			raspi.setYellow(outPut);		

			// Punainen LED
			outPut = 0;														
			if((elPrice > outOn.getLowLimit("Red")) && (elPrice <= outOn.getHighLimit("Red")))
				outPut = 1;
			raspi.setRed(outPut);					
			
			exMin = cMin;	
		}

	}
	return 0;
}

int Control::cntrlOff() { // Ohjaus pysähdyksissä
	time_t t_prev = 0;
	FILE *file;

	while(1) {
		time_t t = time(NULL);
	
		if((t - t_prev) > 5) { //Suoritetaan 5s välein
			t_prev = t;
			system("touch ./temp/alive"); // Vahtikoiralle ruokaa

			// Tilan vaihto -> start					
			if(file = fopen ("start","r")) {
				fclose(file);
				system("rm start");
				break;
			}	
		}
		
	}
	return 0;	
}


