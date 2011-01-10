/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Data.cpp
 *
 */

#include "Data.hpp"

Data::Data()
{
	//this->a_data.resize(4);
	_DEBUG("Initialisation du module de stockage de donnees", INFORMATION);
}

Data::~Data()
{
	_DEBUG("Destruction du module stockage de donnees", INFORMATION);
}

boost::any Data::get(int nb, DataOption::Place place, int in)
{
	if(place == DataOption::FIRST)
		return this->get(nb, 0);
	else
		return this->get(nb, this->a_data[nb].size()-1-in);
}

boost::any Data::get(int nb, int index)
{
	if(this->a_data[nb].size() < (unsigned int)index+1 || index < 0)
		throw std::out_of_range("ERREUR : Aucune donnée stocke... :/:/:::qsdf:sfd !");

	return this->a_data[nb][index];
}

void Data::set(int nb, boost::any value)
{
	this->a_data[nb].push_back(value);
	//_DEBUG("Ajout d'une donnee au module de stockage de donnees", INFORMATION);
}

