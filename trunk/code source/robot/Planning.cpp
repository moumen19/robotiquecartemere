#include "Planning.hpp"

Planning::Planning(Data & environment, Constraint & constraint, Strategy & strategy) :
a_environmentData(environment),
a_constraint(constraint),
a_strategy(strategy)
{
    /*this->a_environmentData = environment;
    this->a_constraint = constraint;
    this->a_strategy = strategy;*/

    _DEBUG("Initialisation du module de planification", INFORMATION);
}

Planning::~Planning()
{

}

void Planning::clearTrajectory()
{
    this->a_trajectory.clear();
}

void Planning::run()
{

}
