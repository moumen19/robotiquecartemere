/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Robot.cpp
 *
 */

#include "Robot.hpp"
#include <sstream>

Robot::Robot()
{
    this->a_sensorsData = new Data();
    this->a_environmentData = new Data();
    this->a_constraint = new Constraint();

    this->a_dataFusion = new DataFusion(this->a_sensorsData, this->a_environmentData);
    this->a_strategy = new Strategy(this->a_environmentData, this->a_constraint);
    this->a_planning = new Planning(this->a_environmentData, this->a_constraint, this->a_strategy);
    this->a_communication = new Communication(this->a_sensorsData, this->a_environmentData, this->a_constraint, this->a_planning);

    this->a_thread_active = false;

    _DEBUG("Initialisation du robot", INFORMATION);
}

Robot::~Robot()
{
    if(this->a_communication->isActive())
        this->a_communication->stop();
    this->stop();
    pthread_join(this->a_thread, NULL);
}

bool Robot::isCommunicationActive()
{
    return this->a_communication->isActive();
}

void Robot::startCommunication()
{
    this->a_communication->start();
}

void Robot::stopCommunication()
{
    this->a_communication->stop();
}

bool Robot::isActive()
{
    return this->a_thread_active;
}

void Robot::start()
{
    this->a_thread_active = true;
    pthread_create(&(this->a_thread), NULL, &Robot::run, (void *)this);
    //run((void *)this);
}

void Robot::stop()
{
    this->a_thread_active = false;
}

void Robot::wait()
{
    pthread_join(this->a_thread, NULL);
}

void * Robot::run(void * data)
{
    Robot* This = static_cast<Robot*>(data);

    _DEBUG("Debut de la routine de calcul des trajectoires", INFORMATION);

    double donnee = 20.01;
    This->a_sensorsData->set(0, donnee);
    donnee = 10.2;
    This->a_sensorsData->set(0, donnee);
    std::cout << boost::any_cast<double>(This->a_sensorsData->get(0, DataOption::LAST)) << std::endl;
    std::cout << boost::any_cast<double>(This->a_sensorsData->get(0, DataOption::FIRST)) << std::endl;

    while(This->a_thread_active)
    {
        //_DEBUG("Robot", INFORMATION);
    }

    _DEBUG("Fin de la routine de calcul des trajectoires", INFORMATION);

    return NULL;
}
