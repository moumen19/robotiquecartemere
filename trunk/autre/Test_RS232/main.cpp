#include <SerialStream.h> // requires libserial package to be installed
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

#include "OpeningAndChecks.cpp"
#include "joystick.c"

using namespace std;
using namespace LibSerial;


extern SerialStream my_serial_port;

int main()
{

    //SerialStream my_serial_port;
    cout<<"START\nOpening serial port...\n";

    // Open Serial
    if(createCOMport(my_serial_port, "/dev/ttyUSB0", SerialStreamBuf::BAUD_9600) ){ // if(my_serial_port.IsOpen()){
        cout<<":)";
    }
    else{
        cout<<":(";
        return 0;
    }

    while(1)
    {
            // coder get Postion
        sendMotorsSpeed(2,4);

    }

    // Close Serial
    my_serial_port.Close();
    cout<<"\nEND\n";
    return 0;
}
