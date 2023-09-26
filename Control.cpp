#ifndef CONTROL
#define CONTROL
#include "Control.hpp"
#endif

Control::Control() {
	cout << "Control: Rakentaja" << endl;
	
	sp = new SpotPrices;	
}

Control::~Control() {
	cout << "Control: Purkaja" << endl;
	
	delete sp;
	sp = NULL;
}

int Control::cntrlLoop() { // Ohjaus
	int exMin = 0, cMin, ret;
	bool outPut = 0;
	float elPrice;
	while(1) {
		time_t t = time(NULL);
		tm *lTime = localtime(&t);
		
		while( exMin != (cMin = lTime->tm_min)) { // Suoritetaan kerran minuutissa
			cout << "Control: cntrlLoop " << cMin << endl;
			
			if((time(NULL) - lastUpdate) > updateTime) { // Päivitetään hinnat
				ret = sp->updatePrices();
				if(!ret) {
					sp->savePrices("ElPrices_.csv"); // Tallennetaan hinnat tiedostoon
					lastUpdate = time(NULL);
				}
			}
			
			elPrice = sp->getPrice(time(NULL));
			
			if((elPrice > winOut[0]) && (elPrice <= winOut[1]))
				outPut = 1;
			else
				outPut = 0;
				
			cout << "Price: " << elPrice << " Out: " << outPut << endl;
			
			RasPiIO raspi;
			//raspi.setOut(outPut);
			
			exMin = cMin;	
		}
	}
	return 0;
}
