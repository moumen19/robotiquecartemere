#include "Communication.hpp"

Communication::Communication(Data *sensors, Data *environment, Constraint *constraint, Planning *planning)
{
    a_sensorsData = sensors;
    a_environmentData = environment;
    a_constraint = constraint;
    a_planning = planning;

    _DEBUG("Communication.cpp", "Initialisation du module de communication", INFORMATION);
}

Communication::~Communication()
{

}

void Communication::send(Port::Port port, std::string msg)
{

}

void Communication::receive()
{

}
