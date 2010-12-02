#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

#define sizeTRAME 14


using namespace LibSerial;

SerialStream my_serial_port;

typedef union
{
	float value;
	unsigned char data[4];
}Decoup_float;

typedef struct
{
    unsigned char Id;
	float X;
	float Y;
	float Alpha;
	unsigned char commande;
}StructPos;

struct Position
{
    float X;
    float Y;
};

void pointToData(StructPos point, unsigned char data[14])
{
	int i;
	Decoup_float x,y,a;

    data[0] = point.Id;

	x.value = point.X;
	for (i=0; i <4; i++)
	{
		data[i+1] =  x.data[i];
	}
	y.value = point.Y;
	for (i=0; i <4; i++)
	{
		data[5+i] =  y.data[i];
	}

	a.value = point.Alpha;
	for (i=0; i <4; i++)
	{
		data[9+i] =  a.data[i];
	}
	data[13] = point.commande;
}

void dataToConsigne(StructPos *point, unsigned char data[14])
{
	int i;
	Decoup_float x;
	for (i=0; i <4; i++)
	{
		x.data[i] = data[i+1];
	}
	point->X = x.value;
	for (i=0; i <4; i++)
	{
		x.data[i] = data[5+i];
	}
	point->Y =x.value;
	for (i=0; i <4; i++)
	{
		x.data[i] = data[9+i];
	}
	point->Alpha = x.value;
	point->commande = data[13];
}

bool createCOMport(SerialStream &serial_port, std::string COMport_name, SerialStreamBuf::BaudRateEnum baudRate)
{
        serial_port.Open(COMport_name);
        if(!serial_port.IsOpen() || !serial_port.good())
        {
            std::cerr << "Pb port IsOpen";
            return false;
        }

        //
        // Set the baud rate of the serial port.
        //
        serial_port.SetBaudRate(baudRate) ;
        if ( ! serial_port.good() )
        {
            std::cerr << "Error: Could not set the baud rate." << std::endl ;
            return false;
        }
        //
        // Set the number of data bits.
        //
        serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
        if ( ! serial_port.good() )
        {
            std::cerr << "Error: Could not set the character size." << std::endl ;
            return false;
        }
        //
        // Disable parity.
        //
        serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
        if ( ! serial_port.good() )
        {
            std::cerr << "Error: Could not disable the parity." << std::endl ;
            return false;
        }
        //
        // Set the number of stop bits.
        //
        serial_port.SetNumOfStopBits( 1 ) ;
        if ( ! serial_port.good() )
        {
            std::cerr << "Error: Could not set the number of stop bits."<< std::endl ;
            return false;
        }
        //
        // Turn off hardware flow control.
        //
        serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ; //FLOW_CONTROL_HARD
        if ( ! serial_port.good() )
        {
            std::cerr << "Error: Could not use hardware flow control."<< std::endl ;
            return false;
        }

        //my_serial_port.SetVMin(10);
        std::cout <<"current size of character buffer : " <<serial_port.VMin () <<std::endl;
        std::cout <<"current timing of character buffer in 10th of a second : " <<serial_port.VTime () <<std::endl;

        return true;
}

void sendMotorsSpeed(float leftWheel, float rightWheel)
 {
        StructPos Commande_position;
        uint8_t buf[sizeTRAME] ;

        // just to initialize
        Commande_position.Id = 42;
        Commande_position.Alpha = 0;

        // command to send
        Commande_position.X = leftWheel;
        Commande_position.Y = rightWheel;
        Commande_position.commande = 4;

        pointToData(Commande_position, buf);
        for(int i=0; i<sizeTRAME; i++)
        {
            my_serial_port << buf[i];
        }
        usleep(100000);
 }

void sendStop()
{
        StructPos Commande_position;
        uint8_t buf[sizeTRAME] ;

        // just to initialize
        Commande_position.Id = 42;
        Commande_position.Alpha = 0;
        Commande_position.X = 0;
        Commande_position.Y = 0;

        // command to send
        Commande_position.commande = 3;

        pointToData(Commande_position, buf);
        for(int i=0; i<sizeTRAME; i++)
        {
            my_serial_port << buf[i];
        }

        usleep(100000);
}

