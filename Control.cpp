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
	
	while(1) {
		time_t t = time(NULL);
		tm *lTime = localtime(&t);
	
		if((t - t_prev) > 5) { //Suoritetaan 5s välein
			t_prev = t;
			
			cout << "Control: cntrlOn" << endl;
			// Tähän lopetus		
			if(file = fopen ("stop","r")) {
				fclose(file);
				system("rm stop");
				break;
			}	
			
			//raspi.setHbeat(heartBeat);
			heartBeat = !heartBeat;
//			cout << heartBeat << endl;			
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
			if((elPrice > winOut[0]) && (elPrice <= winOut[1]))
				outPut = 1;
			cout << "Price: " << elPrice << " Out: " << outPut << endl;			
			//raspi.setOut(outPut);
			
			// Vihreä LED
			outPut = 0;														
			if((elPrice > winGreen[0]) && (elPrice <= winGreen[1]))
				outPut = 1;
			//raspi.setGreen(outPut);		
			
			// Keltainen LED
			outPut = 0;														
			if((elPrice > winYellow[0]) && (elPrice <= winYellow[1]))
				outPut = 1;
			//raspi.setYellow(outPut);		

			// Punainen LED
			outPut = 0;														
			if((elPrice > winRed[0]) && (elPrice <= winRed[1]))
				outPut = 1;
			//raspi.setRed(outPut);					
			
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

			cout << "Control: cntrlOff" << endl;
			
			if(file = fopen ("start","r")) {
				fclose(file);
				system("rm start");
				break;
			}	
		}
		
	}
	return 0;	
}


