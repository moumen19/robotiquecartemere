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
	a_strategySave = NONE;
	_DEBUG("Initialisation du module de strategie", INFORMATION);
}

Strategy::~Strategy()
{
	_DEBUG("Destruction du module de strategie", INFORMATION);
}

Strat Strategy::get()
{
	return this->a_strategy;
}

void Strategy::set(Strat strategy)
{
	switch(strategy)
	{
		case BAU_START:			
			bau_start();
			_DEBUG("BAU_START actif", INFORMATION);
			break;
		case BAU_STOP:
			bau_start();
			_DEBUG("BAU_STOP actif", INFORMATION);
			break;
		case GO_AHEAD:
			go_ahead();
			_DEBUG("GO_AHEAD actif", INFORMATION);
		default:
			break;
	}
}

void Strategy::bau_start()
{
	this->a_strategySave = this->a_strategy;
	this->a_strategy = BAU_START;
}

void Strategy::bau_stop()
{
	this->set(a_strategySave);
}

void Strategy::go_ahead()
{
	this->a_strategy = GO_AHEAD;
}

