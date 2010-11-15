#include "Strategy.hpp"

Strategy::Strategy(Data *environment, Constraint *constraint)
{
    a_constraint = constraint;
    a_environmentData = environment;

    _DEBUG("Strategy.cpp", "Initialisation du module de strategie", INFORMATION);
}

Strategy::~Strategy()
{

}

int Strategy::get()
{
    return a_strategy;
}

void Strategy::set(int strategy)
{
    a_strategy = strategy;
}
