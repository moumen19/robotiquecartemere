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
    a_sensorsData = new Data();
    a_environmentData = new Data();
    a_constraint = new Constraint();

    a_dataFusion = new DataFusion(a_sensorsData, a_environmentData);
    a_strategy = new Strategy(a_environmentData, a_constraint);
    a_planning = new Planning(a_environmentData, a_constraint, a_strategy);
    a_communication = new Communication(a_sensorsData, a_environmentData, a_constraint, a_planning);

    a_thread_active = false;

    a_communication->start();

    _DEBUG("Initialisation du robot", INFORMATION);
}

Robot::~Robot()
{
    if(a_communication->isActive())
        a_communication->stop();
    stop();
    pthread_join(a_thread, NULL);
}

bool Robot::isActive()
{
    return a_thread_active;
}

void Robot::start()
{
    a_thread_active = true;
    pthread_create(&a_thread, NULL, &Robot::run, (void *)this);
    //run((void *)this);
}

void Robot::stop()
{
    a_thread_active = false;
}

void Robot::wait()
{
    pthread_join(a_thread, NULL);
}

void * Robot::run(void * data)
{
    Robot* This = static_cast<Robot*>(data);

    _DEBUG("Debut de la routine de calcul des trajectoires", INFORMATION);

    /*double donnee = 20.01;
    This->a_sensorsData->set(0, &donnee);
    donnee = 10.2;
    This->a_sensorsData->set(0, &donnee);
    std::cout << *((double *)This->a_sensorsData->get(0, DataOption::LAST)) << std::endl;
    std::cout << *((double *)This->a_sensorsData->get(0, DataOption::FIRST)) << std::endl;*/

    int i = 0;
    while(This->a_thread_active)
    {
        i++;
        //_DEBUG("Robot.cpp", oss.str(), INFORMATION);
    }

    _DEBUG("Fin de la routine de calcul des trajectoires", INFORMATION);

    return NULL;
}
