#ifndef RASPIIO_HPP
#define RASPIIO_HPP
#include "RasPiIO.hpp"
#endif

#define RASPI // Ohjelma on Raspberryssä ja GPIO on käytössä
          

RasPiIO::RasPiIO() {
	cout << "RasPiIO: Rakentaja" << endl;
	
	// Nollataan lähdöt
#ifdef RASPI
	setOut(0); // Ohjauslähtö
	setGreen(0); // Vihreä LED
	setYellow(0); // Keltainen LED
	setRed(0); // Punainen LED
	setHbeat(0); // Heartbeat LED
	setRes(0); // Varaus LED
#endif	
}

RasPiIO::~RasPiIO() {
	cout << "RasPiIO: Purkaja" << endl;
	
	// Nollataan lähdöt
#ifdef RASPI
	setOut(0); // Ohjauslähtö
	setGreen(0); // Vihreä LED
	setYellow(0); // Keltainen LED
	setRed(0); // Punainen LED
	setHbeat(0); // Heartbeat LED
	setRes(0); // Varaus LED	
#endif	
}

int RasPiIO::setOut(bool out) { // Ohjauslähtö
	out = out ^ pinOut_.inv_;
	int ret_ = setOutPin(out, pinOut_.pin_);
	return ret_;
}

int RasPiIO::setGreen(bool out) { // Vihreä LED
	out = out ^ pinGreen_.inv_;
	int ret_ = setOutPin(out, pinGreen_.pin_);
	return ret_;
}

int RasPiIO::setYellow(bool out) { // Keltainen LED
	out = out ^ pinYellow_.inv_;
	int ret_ = setOutPin(out, pinYellow_.pin_);
	return ret_;
}

int RasPiIO::setRed(bool out) { // Punainen LED
	out = out ^ pinRed_.inv_;
	int ret_ = setOutPin(out, pinRed_.pin_);
	return ret_;
}

int RasPiIO::setHbeat(bool out) { // Heartbeat LED
	out = out ^ pinHbeat_.inv_;
	int ret_ = setOutPin(out, pinHbeat_.pin_);
	return ret_;
}

int RasPiIO::setRes(bool out) { // Varaus LED
	out = out ^ pinRes_.inv_;
	int ret_ = setOutPin(out, pinRes_.pin_);
	return ret_;
}


int RasPiIO::setOutPin(bool out, string gp_pin) {
	string sOut_ = "0", sBasePath_ = "/sys/class/gpio/gpio", sPath_;
	sBasePath_.append(gp_pin);
	sBasePath_.append("/");
	
	if(out)
		sOut_ = "1";

#ifdef RASPI
	fstream sysfs_handle;                                  

	sysfs_handle.open ("/sys/class/gpio/export", ios::out);        

	if( sysfs_handle.is_open()){
   	sysfs_handle << gp_pin;   
   	sysfs_handle.close();                                
   }else{
      cout << "RasPiIO: Ei voi avata gpio/export: " << gp_pin << endl;
      return -1;
   }
	sPath_ = sBasePath_;
	sPath_.append("direction");

	time_t bg = time(NULL);
	while (1) {
		sysfs_handle.open (sPath_, ios::out);     
		if(sysfs_handle.is_open()) {
			break;
		}
		else if ((time(NULL) - bg) > 2) { // Timeout
      	cout << "RasPiIO: Ei voi avata gpio/direction: " << gp_pin << endl;
      	return -1;      		
		}	
	}
   sysfs_handle << "out";         
	sysfs_handle.close();                           

	sPath_ = sBasePath_;
	sPath_.append("value");

	bg = time(NULL);
	while (1) {
		sysfs_handle.open (sPath_, ios::out);  
		if(sysfs_handle.is_open()) {
			break;
		}
		else if ((time(NULL) - bg) > 2) { // Timeout
      	cout << "RasPiIO: Ei voi avata gpio/value: " << gp_pin << endl;
      	return -1;
      }
	}
   sysfs_handle << sOut_;
   sysfs_handle.close();
   
#endif	
  	return 0;
}
