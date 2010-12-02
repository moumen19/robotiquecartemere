#include "BusRS232.hpp"
//#include <exception>
#include <string.h>

BusRS232::BusRS232(std::string port, int bufferSize)
{
    this->a_port = port;
    this->a_bufferSize = bufferSize;
    this->a_buffer = new char[this->a_bufferSize];
    this->a_bufferWriteCursor = 0;
    this->a_bufferReadCursor = 0;

    this->a_rs232 = new SerialPort(port);

    this->a_thread_active = false;
    
    _DEBUG("Initialisation du module de communication par bus RS232", INFORMATION);
}



BusRS232::~BusRS232()
{
    this->close();
    pthread_join(this->a_thread, NULL);
}

bool BusRS232::open()
{
    this->close();

    try
    {
	this->a_rs232->Open();
    }
    catch(const std::exception & e)
    {
	_DEBUG(e.what(), WARNING);
    }

    if(this->a_rs232->IsOpen())
    {
	try
	{
    	    this->a_rs232->SetBaudRate(SerialPort::BAUD_9600);
            this->a_rs232->SetCharSize(SerialPort::CHAR_SIZE_8);
            this->a_rs232->SetNumOfStopBits(SerialPort::STOP_BITS_1);
            this->a_rs232->SetParity(SerialPort::PARITY_ODD);
            this->a_rs232->SetFlowControl(SerialPort::FLOW_CONTROL_HARD);
        }
	catch(const std::exception & e)
	{
	    _DEBUG(e.what(), WARNING);
	    return false;
	}

	_DEBUG("Ouverture du port RS232", INFORMATION);

	this->a_thread_active = true;
        pthread_create(&(this->a_thread), NULL, &BusRS232::receive, (void *)this);

	return true;
    }

    _DEBUG("Echec de l'ouverture du port RS232", WARNING);
    return false;
}

void BusRS232::close()
{
    if(this->a_rs232->IsOpen())
    {
	try
	{   
	    this->a_thread_active = false;     
	    this->a_rs232->Close();
	}
	catch(const std::exception & e)
	{	
	    _DEBUG(e.what(), WARNING);
	}
        _DEBUG("Fermerure du port RS232", INFORMATION);
    }
}


void BusRS232::send(boost::any msg)
{
    char * buffer;
    int bufferSize = 0;
    buffer = this->onSend(msg, &bufferSize);

    //this->a_rs232->Write(buffer, bufferSize);
}

void * BusRS232::receive(void * data)
{
    BusRS232* This = static_cast<BusRS232*>(data);

    _DEBUG("Debut de la routine d'ecoute d'un port RS232", INFORMATION);

    while(This->a_thread_active)
    {
	//_DEBUG("RS232", INFORMATION);

	if(This->a_rs232->IsOpen())
	{
    	    unsigned char buffer = 0;
    	    try
	    {
		buffer = This->a_rs232->ReadByte(0);
		This->a_buffer[This->a_bufferWriteCursor] = buffer;
		This->a_bufferWriteCursor++;
		if(This->a_bufferWriteCursor == This->a_bufferReadCursor)
		    This->a_bufferReadCursor++;
 	    }
	    catch(const std::exception & e)
	    {
	        //_DEBUG(e.what(), WARNING);
	    }
        } 
	else
	    _DEBUG("Port RS232 non ouvert", WARNING);
    }

    _DEBUG("Fin de la routine d'ecoute d'un port RS232", INFORMATION);

    return NULL;
}

boost::any BusRS232::getData()
{
    return onReceive();
}

char * BusRS232::onSend(const boost::any & msg, int * bufferSize)
{
    std::string str_msg = boost::any_cast<std::string>(msg);
    *bufferSize = (int)str_msg.size();
    char * buffer = new char[*bufferSize];
    strcpy(buffer, str_msg.c_str());

    return buffer;
}

bool BusRS232::isDataAvailable()
{
    int bufferAvailable = this->a_bufferWriteCursor - this->a_bufferReadCursor;
    if(this->a_bufferWriteCursor < this->a_bufferReadCursor)
	bufferAvailable += this->a_bufferSize;

    if(bufferAvailable > 0)
    	return true;

    return false;
}

boost::any BusRS232::onReceive()
{
    if(!this->isDataAvailable())
	_DEBUG("Pas de donnée disponible...", WARNING);	// A remplacer par une vrai excpetion

    boost::any msg = this->a_buffer[this->a_bufferReadCursor];
    this->a_bufferReadCursor++;
    return msg;

}

