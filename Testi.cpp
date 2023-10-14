#ifndef SHELLYPLUGS
#define SHELLYPLUGS
#include "ShellyPlugS.hpp"
#endif

// Testiohjelma
int main() {
	int ret;
	ShellyPlugS plug01("http://192.168.10.37/");
	
	time_t prev_ = 0, t;
	int cntr_ = 0;
	bool pSet_ = true;
	while (1){
		prev_ = time(NULL);
		while (((t = time(NULL)) - prev_) < 15);

		cntr_++;
		cout << pSet_ << endl;
		ret = plug01.setOut(pSet_);
		pSet_ ? pSet_ = false : pSet_ = true;
		if ( cntr_ > 10) {
			return 0;
		}
	}
}