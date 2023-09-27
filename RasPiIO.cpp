//#include <cstdlib>
//#include <unistd.h>
//#include <string>


#ifndef RASPIIO
#define RASPIIO
#include "RasPiIO.hpp"
#endif

          
using namespace std;   

RasPiIO::RasPiIO() {
	cout << "RasPiIO: Rakentaja" << endl;
	
	// Nollataan lähdöt
	setOut(0); // Ohjauslähtö
	setGreen(0); // Vihreä LED
	setYellow(0); // Keltainen LED
	setRed(0); // Punainen LED
	setHbeat(0); // Heartbeat LED
	setRes(0); // Varaus LED	
}

RasPiIO::~RasPiIO() {
	cout << "RasPiIO: Purkaja" << endl;
	
	// Nollataan lähdöt
	setOut(0); // Ohjauslähtö
	setGreen(0); // Vihreä LED
	setYellow(0); // Keltainen LED
	setRed(0); // Punainen LED
	setHbeat(0); // Heartbeat LED
	setRes(0); // Varaus LED	
}

int RasPiIO::setOut(bool out) { // Ohjauslähtö
	out = out ^ pinOut.inv;
	int ret = setOutPin(out, pinOut.pin);
	return ret;
}

int RasPiIO::setGreen(bool out) { // Vihreä LED
	out = out ^ pinGreen.inv;
	int ret = setOutPin(out, pinGreen.pin);
	return ret;
}

int RasPiIO::setYellow(bool out) { // Keltainen LED
	out = out ^ pinYellow.inv;
	int ret = setOutPin(out, pinYellow.pin);
	return ret;
}

int RasPiIO::setRed(bool out) { // Punainen LED
	out = out ^ pinRed.inv;
	int ret = setOutPin(out, pinRed.pin);
	return ret;
}

int RasPiIO::setHbeat(bool out) { // Heartbeat LED
	out = out ^ pinHbeat.inv;
	int ret = setOutPin(out, pinHbeat.pin);
	return ret;
}

int RasPiIO::setRes(bool out) { // Varaus LED
	out = out ^ pinRes.inv;
	int ret = setOutPin(out, pinRes.pin);
	return ret;
}


int RasPiIO::setOutPin(bool out, string gp_pin) {
	string sOut = "0", sBasePath = "/sys/class/gpio/gpio", sPath;
	sBasePath.append(gp_pin);
	sBasePath.append("/");
	cout << "Path1: " << sBasePath << endl;
	
	if(out)
		sOut = "1";

	fstream sysfs_handle;                                  

	sysfs_handle.open ("/sys/class/gpio/export", ios::out);        

	if( sysfs_handle.is_open()){
   	sysfs_handle<<gp_pin;   
   	cout<<"GPIO " << gp_pin << " opened for EXPORT." << endl;
   	sysfs_handle.close();                                
   }else{
      cout<<"\nCan't open gpio/export\n";
      return -1;
   }
	sPath = sBasePath;
	sPath.append("direction");
	cout << "Path2: " << sPath << endl;
	sysfs_handle.open (sPath, ios::out);     

	if( sysfs_handle.is_open()){
   	sysfs_handle<<"out";         
   	cout<<"GPIO " << gp_pin << " opened as output pin." << endl;
  	 	sysfs_handle.close();                           
   }else{
      cout<<"\nCan't open gpio/direction\n";
      return -1;      
   }
	sPath = sBasePath;
	sPath.append("value");
	cout << "Path3: " << sPath << endl;
	sysfs_handle.open (sPath, ios::out);  

	if( sysfs_handle.is_open()){
      sysfs_handle << sOut;
      cout << "GPIO " << gp_pin << " turned " << sOut  << endl;
      sysfs_handle.close();
   }else{
       cout<<"\nCan't open gpio/export\n";
      return -1;
   }
  	return 0;
}
