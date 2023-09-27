#ifndef CONTROL
#define CONTROL
#include "Control.hpp"
#endif

// Pääohjelma
int main() {
	int ret;
	Control ctrl; // Ohjauslogiikka
	while(1) { // Tilakone
		ret = ctrl.cntrlOn(); // Käynnissä
		ret = ctrl.cntrlOff(); // Pysähdyksissä
	}
	return 0;
}