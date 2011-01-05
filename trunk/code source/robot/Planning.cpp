/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Planning.cpp
 *
 */

#include "Planning.hpp"

Planning::Planning(Data & environment, Constraint & constraint, Strategy & strategy) :
	a_environmentData(environment),
	a_constraint(constraint),
	a_strategy(strategy)
{
	a_lastStrategy = NONE;
	_DEBUG("Initialisation du module de planification", INFORMATION);
}

Planning::~Planning()
{
	_DEBUG("Destruction du module de planification", INFORMATION);
}

void Planning::clearTrajectory()
{
	this->a_trajectory.clear();
}

void Planning::run()
{
	switch(a_strategy.get())
	{
		case BAU_ON:
			break;
		case BAU_OFF:
			break;
		case GO_AHEAD:
			break;
		default:
			_DEBUG("La strategie ne correspond à aucune", WARNING);
	}

}

