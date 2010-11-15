#include "Robot.hpp"

Robot::Robot()
{
    a_sensorsData = new Data();
    //a_sensorsData->set(DataOption::IR_10, 10.10);
    a_environmentData = new Data();
    a_constraint = new Constraint();

    a_dataFusion = new DataFusion(a_sensorsData, a_environmentData);
    a_strategy = new Strategy(a_environmentData, a_constraint);
    a_planning = new Planning(a_environmentData, a_constraint, a_strategy);
    a_communication = new Communication(a_sensorsData, a_environmentData, a_constraint, a_planning);
}

Robot::~Robot()
{

}

void Robot::init()
{

}

void Robot::run()
{

}
