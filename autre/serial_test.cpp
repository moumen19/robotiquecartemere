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
   char next_char = 'e' ;
   SerialStream serial_port ;

   my_serial_port.Open( "/dev/ttyUSB0" );
   my_serial_port.SetBaudRate( SerialStreamBuf::BAUD_9600) ;
   my_serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
   my_serial_port.SetNumOfStopBits(1) ;
   my_serial_port.SetParity( SerialStreamBuf::PARITY_ODD ) ;
   my_serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_HARD ) ;



   while(1)
   {
       usleep(10000);

       //if(1)
      if(my_serial_port.IsOpen() && my_serial_port.rdbuf()->in_avail())// && serial_port.good())
       {
           //f(my_serial_stream.available() > 0)

           my_serial_port >> next_char ; //>> next_char ;
           cout << next_char <<endl;
       }
       else
       {
           cout << ":(";
       }

   }
   return 0;
}
