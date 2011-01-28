/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Suivi.cpp
 *
 */

// Non utilise actuellement
#include "Suivi.hpp"

/**
 * Constructeur initialisant les différents modules du robot
 * @param config - Permet d'executer la fusion de donnees et de la planification dans un thread a part ou dans le programme principale
 */
Suivi::Suivi(Planning & planning, Communication & communication) :
	a_planning(planning),
	a_communication(communication)
{
	_DEBUG("Initialisation du suivi", INFORMATION);
}

/**
 * Destructeur
 */
Suivi::~Suivi()
{
	_DEBUG("Destruction du suivi", INFORMATION);
}



