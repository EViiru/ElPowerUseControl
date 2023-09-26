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
	
}

RasPiIO::~RasPiIO() {
	cout << "RasPiIO: Purkaja" << endl;

}

int RasPiIO::setOut(bool out) {
	cout << "in: " << out << " ";
	out = out ^ pinOut.inv;
	cout << "out: " << out << endl;
	
	string sOut = "0", sBasePath = "/sys/class/gpio/gpio", sPath;
	sBasePath.append(pinOut.pin);
	sBasePath.append("/");
	cout << "Path1: " << sBasePath << endl;
	
	if(out)
		sOut = "1";

	fstream sysfs_handle;                                  

	sysfs_handle.open ("/sys/class/gpio/export", ios::out);        

	if( sysfs_handle.is_open()){
   	sysfs_handle<<pinOut.pin;   
   	cout<<"GPIO " << pinOut.pin << " opened for EXPORT." << endl;
   	sysfs_handle.close();                                
   }else{
      cout<<"\nCan't open gpio/export\n";
   }
	sPath = sBasePath;
	sPath.append("direction");
	cout << "Path2: " << sPath << endl;
	sysfs_handle.open (sPath, ios::out);     

	if( sysfs_handle.is_open()){
   	sysfs_handle<<"out";         
   	cout<<"GPIO " << pinOut.pin << " opened as output pin." << endl;
  	 	sysfs_handle.close();                           
   }else{
      cout<<"\nCan't open gpio/direction\n";
   }
	sPath = sBasePath;
	sPath.append("value");
	cout << "Path3: " << sPath << endl;
	sysfs_handle.open (sPath, ios::out);  

	if( sysfs_handle.is_open()){
      sysfs_handle << sOut;
      cout << "GPIO " << pinOut.pin << " turned " << sOut  << endl;
      sysfs_handle.close();
   }else{
       cout<<"\nCan't open gpio/export\n";
   }
  	return 0;
}
