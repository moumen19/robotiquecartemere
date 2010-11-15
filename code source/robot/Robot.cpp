#include "Robot.hpp"

Robot::Robot()
{
    a_sensorsData = new Data();
    a_environmentData = new Data();
    a_constraint = new Constraint();

    a_dataFusion = new DataFusion(a_sensorsData, a_environmentData);
    a_strategy = new Strategy(a_environmentData, a_constraint);
    a_planning = new Planning(a_environmentData, a_constraint, a_strategy);
    a_communication = new Communication(a_sensorsData, a_environmentData, a_constraint, a_planning);

    if(_DEBUG_MODE)
        _DEBUG::addMessage("Robot.cpp", "Initialisation du robot", INFORMATION);
}

Robot::~Robot()
{

}

void Robot::run()
{

}
