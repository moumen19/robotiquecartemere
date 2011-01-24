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

/**
 * Constructeur
 */
Data::Data()
{
	_DEBUG("Initialisation du module de stockage de donnees", INFORMATION);
}

/**
 * Destructeur
 */
Data::~Data()
{
	_DEBUG("Destruction du module stockage de donnees", INFORMATION);
}

/**
 * Recupere une donnee stockee
 * @param nb - indice de la donnee (indice capteur par exemple)
 * @param place - place de la donnee
 * @param in - indice negatif si la place = LAST. Compte à partir de la fin de la liste
 * @return La donnee telle qu'elle a ete stocke (a caster comme il se doit)
 */
boost::any Data::get(int nb, DataOption::Place place, int in)
{
	if(place == DataOption::FIRST)
		return this->get(nb, 0);
	else
		return this->get(nb, this->a_data[nb].size()-1-in);
}

/**
 * Recupere une donnee stockee
 * @param nb - indice de la donnee (indice capteur par exemple)
 * @param index - numero de la donnee
 * @return La donnee telle qu'elle a ete stocke (a caster comme il se doit)
 */
boost::any Data::get(int nb, int index)
{
	if(this->a_data[nb].size() < (unsigned int)index+1 || index < 0)
		throw std::out_of_range("ERREUR : Aucune donnée stocke...");

	return this->a_data[nb][index];
}

/**
 * Ajout en memoire d'une donnee
 * @param nb - indice de la donnee (indice capteur par exemple)
 * @param value - donnee a stocke
 */
void Data::set(int nb, boost::any value)
{
	this->a_data[nb].push_back(value);
	//_DEBUG("Ajout d'une donnee au module de stockage de donnees", INFORMATION);
}

