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

/** 
 * Constructeur
 * @param environment - le module de stockage de donnees de l'environnement
 * @param constraint - le module de contraintes
 */
Strategy::Strategy(Data & environment, Constraint & constraint) :
	a_constraint(constraint),
	a_environmentData(environment)
{
	a_strategySave = NONE;
	_DEBUG("Initialisation du module de strategie", INFORMATION);
}

/**
 * Destructeur
 */
Strategy::~Strategy()
{
	_DEBUG("Destruction du module de strategie", INFORMATION);
}

/**
 * Methode qui initialise la strategie
 * @param strategy - la strategie
 */
void Strategy::init(Strat strategy)
{
	a_strategySave = strategy;
	this->set(strategy);
}

/**
 * Methode retournant la strategie en cours
 * @return la strategie en cours
 */
Strat Strategy::get()
{
	return this->a_strategy;
}

/**
 * Methode definissant la strategie a adopter
 * @param strategie - la strategie a adopter
 */
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

/**
 * Strategie BAU active
 */
void Strategy::bau_on()
{
	if(this->get() != BAU_ON)
	{
		_DEBUG("Strategie BAU_ON active", INFORMATION);
		this->a_strategySave = this->a_strategy;
		this->a_strategy = BAU_ON;
	}
}

/**
 * Strategie BAU desactive
 */
void Strategy::bau_off()
{
	if(this->a_strategySave != this->get())
	{
		_DEBUG("Strategie BAU_OFF active", INFORMATION);
		this->set(a_strategySave);
	}
}

/**
 * Strategie principale
 */
void Strategy::go_ahead()
{
	_DEBUG("Strategy GO_AHEAD active", INFORMATION);
	this->a_strategy = GO_AHEAD;
}

