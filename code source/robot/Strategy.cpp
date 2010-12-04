/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Strategy.cpp
 *
 */

#include "Strategy.hpp"

Strategy::Strategy(Data & environment, Constraint & constraint) :
	a_constraint(constraint),
	a_environmentData(environment)
{
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

