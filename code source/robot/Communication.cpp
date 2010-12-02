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
    
    //a_bufferAsservissementCursor = 0;
    //a_bufferSensorCursor = 0;

    this->a_asservissement = new BusRS232();
    this->a_asservissement->open();

    //this->a_sensor = new BusRS232();
    //this->a_sensor->open();

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
}

void Communication::send(Port::Port port, std::string msg)
{
    if(port == Port::ASSERVISSEMENT)
    {
	this->a_asservissement->send(msg);
    }
    else if(port == Port::SENSOR)
    {
	//this->a_sensor->send(msg);
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

    //char m;
    //char s[256] = "";
int i = 0;
    while(This->a_thread_active && i < 26)
    {
	char msg;
	if(This->a_asservissement->isDataAvailable())
	{
i++;
	    msg = boost::any_cast<char>(This->a_asservissement->getData());
            std::cout << msg;
	}
        /*if(This->a_rs232Asservissement.IsOpen())
        {
            while(This->a_rs232Asservissement.rdbuf()->in_avail())
            {
                This->a_rs232Asservissement >> m;
                if(m == 42)
                {
                    sprintf(s, "Reception de donnees de la part de l'asservissement : %s", This->a_bufferAsservissement);
                    _DEBUG(s, INFORMATION);
                    This->a_bufferAsservissementCursor = 0;
                }

                if(This->a_bufferAsservissementCursor <= _BUFFER_ASSERVISSEMENT_SIZE)
                {
                    This->a_bufferAsservissement[This->a_bufferAsservissementCursor] = m;
                    This->a_bufferAsservissementCursor++;
                }
                else
                    _DEBUG("Debordement du buffer de reception de donnees asservissement...", WARNING);
            }
        }
        else
            _DEBUG("Erreur de communication avec l'asservissement", WARNING);
*/

    }
This->a_asservissement->close();
    _DEBUG("Fin de la routine d'ecoute des ports de communications", INFORMATION);

    return NULL;
}
