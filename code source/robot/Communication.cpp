#include "Communication.hpp"
#include <sstream>

Communication::Communication(Data *sensors, Data *environment, Constraint *constraint, Planning *planning)
{
    a_sensorsData = sensors;
    a_environmentData = environment;
    a_constraint = constraint;
    a_planning = planning;

    a_thread_active = false;

    _DEBUG("Communication.cpp", "Initialisation du module de communication", INFORMATION);
}

Communication::~Communication()
{
    stop();
    pthread_join(a_thread);
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
    _DEBUG("Communication.cpp", "Debut de la routine d'ecoute des ports de communications", INFORMATION);

    Communication* This = static_cast<Communication*>(data);
    int i = 0;
    while(This->a_thread_active)
    {
        i++;
        std::ostringstream oss;
        oss << "run : " << i;
        _DEBUG("Communication.cpp", oss.str(), INFORMATION);
    }

    _DEBUG("Communication.cpp", "Fin de la routine d'ecoute des ports de communications", INFORMATION);

    return NULL;
}
