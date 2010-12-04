#include "Strategy.hpp"

Strategy::Strategy(Data & environment, Constraint & constraint) :
a_constraint(constraint),
a_environmentData(environment)
{
    /*this->a_constraint = constraint;
    this->a_environmentData = environment;*/

    _DEBUG("Initialisation du module de strategie", INFORMATION);
}

Strategy::~Strategy()
{

}

int Strategy::get()
{
    return this->a_strategy;
}

void Strategy::set(int strategy)
{
    this->a_strategy = strategy;
}
