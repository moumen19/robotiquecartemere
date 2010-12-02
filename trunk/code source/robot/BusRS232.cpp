#include "BusRS232.hpp"
//#include <exception>
#include <string.h>

pthread_mutex_t  BusRS232::a_mutex = PTHREAD_MUTEX_INITIALIZER;

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
	this->a_rs232->Open(SerialPort::BAUD_9600, 
			    SerialPort::CHAR_SIZE_8, 
			    SerialPort::PARITY_ODD, 
			    SerialPort::STOP_BITS_1, 
			    SerialPort::FLOW_CONTROL_NONE);
    }
    catch(const std::exception & e)
    {
	_DEBUG(e.what(), WARNING);
    }

    if(this->a_rs232->IsOpen())
    {
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
 	    //pthread_join(this->a_thread, NULL);	// Attention blockage ici !
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
    SerialPort::DataBuffer buffer = this->onSend(msg);

    this->a_rs232->Write(buffer);
}

SerialPort::DataBuffer BusRS232::onSend(const boost::any & msg)
{
    std::string str_msg = boost::any_cast<std::string>(msg);
    SerialPort::DataBuffer buffer;

    for(unsigned int i = 0; i < str_msg.size(); i++)
	buffer.push_back(str_msg[i]);

    return buffer;
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

		pthread_mutex_lock(&BusRS232::a_mutex);

		This->a_buffer[This->a_bufferWriteCursor] = buffer;
		This->a_bufferWriteCursor++;
		if(This->a_bufferWriteCursor >= This->a_bufferSize)
		    This->a_bufferWriteCursor -= This->a_bufferSize;

		if(This->a_bufferWriteCursor == This->a_bufferReadCursor)
		{
		    This->a_bufferReadCursor++;
		    if(This->a_bufferReadCursor >= This->a_bufferSize)
			This->a_bufferReadCursor -= This->a_bufferSize;
		}

		pthread_mutex_unlock(&BusRS232::a_mutex);
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
	//_DEBUG("RECEIVE", INFORMATION);
    return onReceive();
}

bool BusRS232::isDataAvailable()
{
    pthread_mutex_lock(&BusRS232::a_mutex);

    int bufferAvailable = this->a_bufferWriteCursor - this->a_bufferReadCursor;
    if(this->a_bufferWriteCursor < this->a_bufferReadCursor)
	bufferAvailable += this->a_bufferSize;

    pthread_mutex_unlock(&BusRS232::a_mutex);
		
    if(bufferAvailable > 0)
    	return true;

    return false;
}

boost::any BusRS232::onReceive()
{
    if(!this->isDataAvailable())
	_DEBUG("Pas de donnée disponible...", WARNING);	// A remplacer par une vrai excpetion

    pthread_mutex_lock(&BusRS232::a_mutex);
		
    boost::any msg = this->a_buffer[this->a_bufferReadCursor];
    this->a_bufferReadCursor++;

    pthread_mutex_unlock(&BusRS232::a_mutex);
		
    return msg;

}

