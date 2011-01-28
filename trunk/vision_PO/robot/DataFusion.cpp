/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier DataFusion.cpp
 *
 */

// Non utilise actuellement
#include "DataFusion.hpp"

DataFusion::DataFusion(Data & sensor, Data & environment) :
	a_sensorsData(sensor),
	a_environmentData(environment)
{
	_DEBUG("Initialisation du module de fusion de donnees", INFORMATION);
}

DataFusion::~DataFusion()
{
	_DEBUG("Destruction du module fusion de donnees", INFORMATION);
}

void DataFusion::run()
{

}

