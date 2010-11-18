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
    a_sensorsData = sensors;
    a_environmentData = environment;
    a_constraint = constraint;
    a_planning = planning;

    a_thread_active = false;

    _DEBUG("Initialisation du module de communication", INFORMATION);
}

Communication::~Communication()
{
    stop();
    pthread_join(a_thread, NULL);
}

void Communication::send(Port::Port port, std::string msg)
{

}

void Communication::receive()
{

}

bool Communication::isActive()
{
    return a_thread_active;
}

void Communication::start()
{
    a_thread_active = true;
    pthread_create(&a_thread, NULL, &Communication::run, (void *)this);
}

void Communication::stop()
{
    a_thread_active = false;
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
