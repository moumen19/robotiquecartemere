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


int main(int argc, char *argv[])
{
   int fd;
   int done = 0;
   int left,right;
    left = 5;
    right = 5;
   fd = open_joystick("");
   if (fd < 0) {
      printf("open failed.\n");
      exit(1);
   }

    //*/ Open Serial
    if(createCOMport(my_serial_port, "/dev/ttyUSB0", SerialStreamBuf::BAUD_9600) ){ // if(my_serial_port.IsOpen()){
        cout<<":)"<<endl;
    }
    else{
        cout<<":("<<endl;
        exit(1);
    }//*/

   while (!done ) {

        GetMotorSpeed(&left,&right);
        printf("left : %d\nright : %d\n",left,right);
        sendMotorsSpeed((float)left,(float)right);
        usleep(10000);
   }

   /*while (!done ) {

        //GetMotorSpeed(&left,&right);
        //printf("left : %d\nright : %d\n",left,right);
        sendMotorsSpeed((float)left,(float)right);
        sleep(1);
   }*/

     // Close Serial
    my_serial_port.Close();
    cout<<"\nEND\n";
    return 0;
}


/*int main(int argc, char *argv[])
{
   int fd;
   int done = 0;
   int left,right;
    left = 0;
    right = 0;

     // Open Serial
    if(createCOMport(my_serial_port, "/dev/ttyUSB0", SerialStreamBuf::BAUD_9600) ){ // if(my_serial_port.IsOpen()){
        cout<<":)"<<endl;
    }
    else{
        cout<<":("<<endl;
        exit(1);
    }

    // Open Joy
   fd = open_joystick("");
   if (fd < 0) {
      printf("open failed.\n");
      exit(1);
   }

   while (!done) {

        GetMotorSpeed(&left,&right);
        //system("clear");
        printf("left : %d\nright : %d\n",left,right);
        sendMotorsSpeed(left,right);
        //usleep(10000);
   }

     // Close Serial
    my_serial_port.Close();
    cout<<"\nEND\n";
    return 0;
}*/

