#include <SerialStream.h> // requires libserial package to be installed
///////////////////////////////// DOC ////////////////////////////////////
//
// http://libserial.sourceforge.net/doxygen/index.html
// http://libserial.sourceforge.net/mediawiki/index.php/LibSerial_Tutorial
//
// g++ read_port.cpp -L/usr/local/lib/  -lserial
//
//////////////////////////////////////////////////////////////////////////
#include <iostream>

using namespace std;
using namespace LibSerial;

int main()
{
    SerialStream my_serial_port;

    char stop = 'c';
    char next_char = 'e' ;

    cout<<"START\n";

    sleep(1);

    my_serial_port.Close();

    my_serial_port.Open("/dev/ttyUSB0");

    if(my_serial_port.IsOpen())
    {
        cout<<":)"<<endl;
    }
    else
    {
        cout<<":("<<endl;
    }

    sleep(2); // petit delay pour initialiser

    my_serial_port.SetBaudRate( SerialStreamBuf::BAUD_9600) ;
    my_serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    my_serial_port.SetNumOfStopBits(1) ;
    my_serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    my_serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_HARD ) ;

    cout<< "current size of character buffer : " <<my_serial_port.VMin () <<endl;
    cout<< "current timing of character buffer in 10th of a second : " <<my_serial_port.VTime () <<endl;

    my_serial_port.SetVMin(10);

    cout<< "current size of character buffer : " <<my_serial_port.VMin () <<endl;
    cout<< "current timing of character buffer in 10th of a second : " <<my_serial_port.VTime () <<endl;

    while(getchar() != '\n')
    {
       //usleep(100);

      if(!my_serial_port.IsOpen())
       {
            cout<< "Pb port IsOpen";
            break;
        }

        if(my_serial_port.rdbuf()->in_avail()) // && serial_port.good())
        {
            //f(my_serial_stream.available() > 0)

            my_serial_port >> next_char ;
            cout << next_char <<endl;
        }
    }

    sleep(1);
    my_serial_port.Close();
    sleep(1);

    return 0;
}


