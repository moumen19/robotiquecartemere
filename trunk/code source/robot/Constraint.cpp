/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Constraint.cpp
 *
 */

#include "Constraint.hpp"

Constraint::Constraint()
{
	_DEBUG("Initialisation du module de stockage des contraintes", INFORMATION);
}

Constraint::~Constraint()
{
	_DEBUG("Destruction du module de contraintes", INFORMATION);
}

double Constraint::get(int nb)
{
	return this->a_data[nb];
}

void Constraint::set(int nb, double value)
{
	this->a_data[nb] = value;
}

