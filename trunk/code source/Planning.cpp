#include "Planning.hpp"

Planning::Planning(Data *environment, Constraint *constraint, Strategy *strategy)
{
    a_environmentData = environment;
    a_constraint = constraint;
    a_strategy = strategy;
}

Planning::~Planning()
{

}

void Planning::clearTrajectory()
{
    a_trajectory.clear();
}

void Planning::run()
{

}
