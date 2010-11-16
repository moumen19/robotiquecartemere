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

    _DEBUG("Robot.cpp", "Initialisation du robot", INFORMATION);
}

Robot::~Robot()
{
    if(a_communication->isActive())
        a_communication->stop();
    stop();
    pthread_join(a_thread);
}

bool Robot::isActive()
{
    return a_thread_active;
}

void Robot::start()
{
    a_thread_active = true;
    pthread_create(&a_thread, NULL, &Robot::run, (void *)this);
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
    _DEBUG("Robot.cpp", "Debut de la routine de calcul des trajectoires", INFORMATION);

    Robot* This = static_cast<Robot*>(data);
    int i = 0;
    while(This->a_thread_active)
    {
        i++;
        std::ostringstream oss;
        oss << "run : " << i;
        _DEBUG("Robot.cpp", oss.str(), INFORMATION);
    }

    _DEBUG("Robot.cpp", "Fin de la routine de calcul des trajectoires", INFORMATION);

    return NULL;
}
