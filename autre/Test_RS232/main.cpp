#include <SerialStream.h> // requires libserial package to be installed
#include <SerialPort.h>
///////////////////////////////// DOC ////////////////////////////////////

//
// http://libserial.sourceforge.net/doxygen/index.html
// http://libserial.sourceforge.net/mediawiki/index.php/LibSerial_Tutorial
// http://www.unixwiz.net/techtips/termios-vmin-vtime.html
//
// g++ read_port.cpp -L/usr/local/lib/  -lserial
//
//////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <stdint.h>

#include "BusRS232.hpp"
#include "OpeningAndChecks.cpp"

using namespace std;
using namespace LibSerial;


extern SerialStream my_serial_port;
extern SerialPort portS(const std::string COMport_name);


int main(int argc, char *argv[])
{
   int fd;
   int done = 0;
   int left,right;
   char a = 'j';
   left = 5;
    right = 5;

   // BusRS232 bus = BusRS232();

    //*/ Open Serial
    if(createCOMport(portS("/dev/ttyUSB0"), SerialPort::BAUD_9600) ){ // if(my_serial_port.IsOpen()){
        cout<<":)"<<endl;
    }
    else{
        cout<<":("<<endl;
        exit(1);
    }//*/

   while (!done ) {

    for(int i=0;i<7;i++){
        my_serial_port >> a;
        cout<<a<<endl;
    }
        usleep(100000);
   }


     // Close Serial
    my_serial_port.Close();
    cout<<"\nEND\n";
    return 0;
}
