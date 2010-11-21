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
    this->a_rs232 = new BusRS232();
    this->a_rs232->open();  //this->a_rs232->open("COM1");
    this->a_i2c = new BusI2C();

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

    delete this->a_rs232;
    delete this->a_i2c;
}

void Communication::send(Port::Port port, std::string msg)
{

}

void Communication::receive()
{

}

bool Communication::isActive()
{
    return this->a_thread_active;
}

void Communication::start()
{
    this->a_thread_active = true;
    pthread_create(&(this->a_thread), NULL, &Communication::run, (void *)this);
}

void Communication::stop()
{
    this->a_thread_active = false;
}

void * Communication::run(void * data)
{
    Communication* This = static_cast<Communication*>(data);
    int i = 0;

    _DEBUG("Debut de la routine d'ecoute des ports de communications", INFORMATION);

    while(This->a_thread_active)
    {
        i++;
        //_DEBUG(oss.str(), INFORMATION);
    }

    _DEBUG("Fin de la routine d'ecoute des ports de communications", INFORMATION);

    return NULL;
}
