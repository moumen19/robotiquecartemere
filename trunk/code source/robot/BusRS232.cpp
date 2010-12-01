#include "BusRS232.hpp"
#include <string.h>

BusRS232::BusRS232()
{
    this->a_port = "";

    _DEBUG("Initialisation du module de communication par bus RS232", INFORMATION);
}



BusRS232::~BusRS232()
{
    this->close();
}

bool BusRS232::open(std::string port)
{
    this->a_port = port;
    this->close();

    this->a_rs232.Open("/dev/ttyUSB0");
    if(this->a_rs232.IsOpen())
    {
        this->a_rs232.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_9600);
        this->a_rs232.SetCharSize(LibSerial::SerialStreamBuf::CHAR_SIZE_8);
        this->a_rs232.SetNumOfStopBits(1);
        this->a_rs232.SetParity(LibSerial::SerialStreamBuf::PARITY_ODD);
        this->a_rs232.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_HARD);

	_DEBUG("Ouverture du port RS232", INFORMATION);
	return false;
    }

    _DEBUG("Echec de l'ouverture du port RS232", WARNING);
    return false;
}

void BusRS232::close()
{
    if(a_rs232.IsOpen())
    {
        a_rs232.Close();
        _DEBUG("Fermerure du port RS232", INFORMATION);
    }
}


void BusRS232::send(boost::any msg)
{
    char * buffer;
    int bufferSize = 0;
    buffer = this->onSend(msg, &bufferSize);

    this->a_rs232.write(buffer, bufferSize);
}

boost::any BusRS232::receive()
{
    int bufferSize = 1;
    char * buffer = new char[bufferSize];
    
    this->a_rs232.read(buffer, bufferSize);
    return onReceive(buffer, bufferSize);
}

char * BusRS232::onSend(const boost::any & msg, int * bufferSize)
{
    std::string str_msg = boost::any_cast<std::string>(msg);
    *bufferSize = (int)str_msg.size();
    char * buffer = new char[*bufferSize];
    strcpy(buffer, str_msg.c_str());

    return buffer;
}

boost::any BusRS232::onReceive(const char * buffer, int bufferSize)
{
    boost::any msg = std::string(buffer, bufferSize);
    return msg;
}

