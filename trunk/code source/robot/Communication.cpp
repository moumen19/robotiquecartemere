/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Communication.cpp
 *
 */

#include "Communication.hpp"
#include <sstream>

Communication::Communication(Data *sensors, Data *environment, Constraint *constraint, Planning *planning)
{
    this->a_rs232Asservissement.Open("/dev/ttyUSB0");
    if(this->a_rs232Asservissement.IsOpen())
    {
        this->a_rs232Asservissement.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_9600);
        this->a_rs232Asservissement.SetCharSize(LibSerial::SerialStreamBuf::CHAR_SIZE_8);
        this->a_rs232Asservissement.SetNumOfStopBits(1);
        this->a_rs232Asservissement.SetParity(LibSerial::SerialStreamBuf::PARITY_ODD);
        this->a_rs232Asservissement.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_HARD);
    }
    else
	_DEBUG("Echec de l'ouverture du port RS232 avec l'asservissement", WARNING);

    if(this->a_rs232Sensor.IsOpen())
    {
        this->a_rs232Sensor.Open("/dev/ttyUSB1");
        this->a_rs232Sensor.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_9600);
        this->a_rs232Sensor.SetCharSize(LibSerial::SerialStreamBuf::CHAR_SIZE_8);
        this->a_rs232Sensor.SetNumOfStopBits(1);
        this->a_rs232Sensor.SetParity(LibSerial::SerialStreamBuf::PARITY_ODD);
        this->a_rs232Sensor.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_HARD);
    }
    else
	_DEBUG("Echec de l'ouverture du port RS232 avec les capteurs", WARNING);

    this->a_sensorsData = sensors;
    this->a_environmentData = environment;
    this->a_constraint = constraint;
    this->a_planning = planning;

    this->a_thread_active = false;

    _DEBUG("Initialisation du module de communication", INFORMATION);
}

Communication::~Communication()
{
    this->stop();
    pthread_join(this->a_thread, NULL);

//    delete this->a_rs232;
//    delete this->a_i2c;
}

void Communication::send(Port::Port port, std::string msg)
{
    if(port == Port::ASSERVISSEMENT)
    {
	this->a_rs232Asservissement << msg.c_str();
    }
    else if(port == Port::SENSOR)
    {
	this->a_rs232Sensor << msg.c_str();
    }
    else
	_DEBUG("Envoie des données à un port non existant !", WARNING);
}

/*void Communication::receive()
{

}*/

bool Communication::isActive()
{
    return this->a_thread_active;
}

void Communication::start()
{
    this->a_thread_active = true;
    //a_thread = new boost::thread(&Communication::run, this);
    pthread_create(&(this->a_thread), NULL, &Communication::run, (void *)this);
}

void Communication::stop()
{
    this->a_thread_active = false;
}

void * Communication::run(void * data)
{
    Communication* This = static_cast<Communication*>(data);

    _DEBUG("Debut de la routine d'ecoute des ports de communications", INFORMATION);

    char m;
    char s[256];

    while(This->a_thread_active)
    {
	if(This->a_rs232Asservissement.IsOpen() && This->a_rs232Asservissement.rdbuf()->in_avail())
	{
            This->a_rs232Asservissement >> m;
	    sprintf(s, "Reception de donnees de la part de l'asservissement : %c", m);
	    _DEBUG(s, INFORMATION);
	}

	if(This->a_rs232Sensor.IsOpen() && This->a_rs232Sensor.rdbuf()->in_avail())
	{
	    This->a_rs232Asservissement >> m;
	    sprintf(s, "Reception de donnees de la part des capteurs : %c", m);
	    _DEBUG(s, INFORMATION);
	}
    }

    _DEBUG("Fin de la routine d'ecoute des ports de communications", INFORMATION);

    return NULL;
}
