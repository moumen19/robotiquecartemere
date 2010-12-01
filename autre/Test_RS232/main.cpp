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
#include <exception>

#include "OpeningAndChecks.cpp"

using namespace std;
using namespace LibSerial;

typedef union
{
	float value;
	unsigned char data[4];
}Decoup_float;

typedef struct
{
	float X;
	float Y;
	float Alpha;
	unsigned char commande;
}StructPos;


void pointToData(StructPos point, unsigned char data[13])
{
	int i;
	Decoup_float x,y,a;

	x.value = point.X;
	for (i=0; i <4; i++)
	{
		data[i] =  x.data[i];
	}
	y.value = point.Y;
	for (i=0; i <4; i++)
	{
		data[4+i] =  y.data[i];
	}

	a.value = point.Alpha;
	for (i=0; i <4; i++)
	{
		data[8+i] =  a.data[i];
	}
	data[12] = point.commande;
}

void dataToConsigne(StructPos *point, unsigned char data[13])
{
	int i;
	Decoup_float x;
	for (i=0; i <4; i++)
	{
		x.data[i] = data[i];
	}
	point->X = x.value;
	for (i=0; i <4; i++)
	{
		x.data[i] = data[4+i];
	}
	point->Y =x.value;
	for (i=0; i <4; i++)
	{
		x.data[i] = data[8+i];
	}
	point->Alpha = x.value;
	point->commande = data[12];
}

int main()
{
    int count=0;
    vector<unsigned char> vect;
    unsigned char a = 'o';
    unsigned char buf[13] ;
    StructPos Commande_position;


        SerialStream my_serial_port;
        cout<<"START\n";
        cout << buf;
       // sleep(0.1);

        if(createCOMport(my_serial_port, "/dev/ttyUSB0", SerialStreamBuf::BAUD_9600) ){
        // if(my_serial_port.IsOpen()){
            cout<<":)";
        }
        else{
            cout<<":(";
        }


        for(int i=0;i<3;i++)
        {

                sleep(1);
                Commande_position.commande=7;
                pointToData(Commande_position, buf);
                for(int i=0; i<13; i++)
                {
                    my_serial_port << buf[i];
                    //cout << buf[i];
                }

            //sleep()

            try{
                while(vect.size() < 13 &&  count<1000) //  && my_serial_port.rdbuf()->in_avail()
                {
                    //if(my_serial_port.)
                   my_serial_port >> a ;
                   vect.push_back(a);
                   count++;
                   cout<<count<< "     "<<a<<endl;
                }
            }
            catch (exception& e)
            {
                cout << "\nStandard exception: " << e.what() << endl;
            }


        }

        my_serial_port.Close();

        for(int i=0;i<vect.size();i++){
            cout<<vect[i];
        }

        for(int i=0;i<vect.size();i++){
            buf[i] = vect[i];
        }
        vect.clear();
        dataToConsigne(&Commande_position, buf);
        cout<<"x: "<<Commande_position.X<<endl;
        cout<<"Y: "<<Commande_position.Y<<endl;
        cout<<"A: "<<Commande_position.Alpha<<endl;
        cout<<"C:"<<Commande_position.commande<<endl;


      // }

    cout<<"count:" <<count<<"\nEND\n";
    return 0;
}
//*/

