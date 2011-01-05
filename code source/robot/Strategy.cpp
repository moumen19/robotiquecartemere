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

void Strategy::init(Strat strategy)
{
	a_strategySave = strategy;
	this->set(strategy);
}

Strat Strategy::get()
{
	return this->a_strategy;
}

void Strategy::set(Strat strategy)
{
	switch(strategy)
	{
		case BAU_ON:			
			bau_on();
			break;
		case BAU_OFF:
			bau_off();
			break;
		case GO_AHEAD:
			go_ahead();
			break;
		default:
			break;
	}
}

void Strategy::bau_on()
{
	if(this->get() != BAU_ON)
	{
		_DEBUG("BAU_ON actif", INFORMATION);
		this->a_strategySave = this->a_strategy;
		this->a_strategy = BAU_ON;
	}
}

void Strategy::bau_off()
{
	if(this->a_strategySave != this->get())
	{
		_DEBUG("BAU_OFF actif", INFORMATION);
		this->set(a_strategySave);
	}
}

void Strategy::go_ahead()
{
	_DEBUG("GO_AHEAD actif", INFORMATION);
	this->a_strategy = GO_AHEAD;
}

