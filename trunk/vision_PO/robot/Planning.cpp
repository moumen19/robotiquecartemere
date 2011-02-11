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
#include "RS232Asservissement.hpp"
#include "RS232Sensor.hpp"
#include "objectTracking.hpp"
#include <math.h>


#include "plan_flou.cc"

/**
 * Constructeur
 * @param environment - module de stockage des donnees d'environnement
 * @param constraint - module de contrainte
 * @param strategy - module de strategie
 * @param sensors - module des capteurs
 */
Planning::Planning(Data & environment, Constraint & constraint, Strategy & strategy, Sensors & sensors) :
	a_environmentData(environment),
	a_constraint(constraint),
	a_strategy(strategy),
	a_sensors(sensors)
{
	//a_lastStrategy = NONE;
	setupTracking();
	_DEBUG("Initialisation du module de planification", INFORMATION);
}

/**
 * Destructeur 
 */
Planning::~Planning()
{
	_DEBUG("Destruction du module de planification", INFORMATION);
}

/**
 * Methode effaçant les points de trajectoire en memoire
 */
void Planning::clearTrajectory()
{
	this->a_trajectory.clear();
}

/**
 * Planifie en fonction de la strategie
 */
void Planning::run()
{
	switch(a_strategy.get())
	{
		// BAU active
		case BAU_ON:
			break;
		// Strategie principale
		case GO_AHEAD:	
			//this->track();
			this->flou();		// Attention
			break;
		default:
			_DEBUG("La strategie ne correspond à aucune", WARNING);
	}

}

/**
 * Methode retournant le dernier point de la trajectoire stocke
 * @return une structure Point
 */
Point Planning::get()
{
	if(a_trajectory.size() == 0)
		throw std::out_of_range("ERREUR : Aucune trajectoire stocke... !");

	return a_trajectory.at(a_trajectory.size()-1);
}


/**
 * Suivi de balle a la camera par régulation proportionnelle
 */
void Planning::track()
{
	int center = 0;
	float correction = 0;
	if(Routine(center))
        {
		center -= 640/2;

		float P = 0.005;
		correction = P*center;

		//*	// On bride la vitesse
		if(correction > 1)
			correction = 1;
		else if(correction < -1)
			correction = -1; //*/
	}

	// On stocke la vitesse
	Point vitesse;
	vitesse.x = correction/2;
	vitesse.y = -1*correction/2;
	a_trajectory.push_back(vitesse);	// Stocke la trajectoire
	
	// Debuggage, affichage des vitesses
	_DISPLAY(vitesse.x << " :: " << vitesse.y << std::endl);
}

/**
 * Plannification floue
 */
void Planning::flou()
{
	float capteurs[10];
	
	capteurs[0] = 0;
	capteurs[1] = 0;
	capteurs[2] = 0;
	capteurs[3] = 0;
	capteurs[4] = 0;
	capteurs[5] = 0;	
	capteurs[6] = 0;
	capteurs[7] = 0;
	capteurs[8] = 0;
	capteurs[9] = 3;
	try
	{
		// Recupere les donnees capteurs
		int moyenne = 1;
		for(int in = 0; in < moyenne; in++)
		{
			messageSensor msg = boost::any_cast<messageSensor>(a_environmentData.get(50, DataOption::LAST, in));
			capteurs[0] += (float)msg.data.getValue()/1000;
			msg = boost::any_cast<messageSensor>(a_environmentData.get(54, DataOption::LAST, in));
			capteurs[1] += (float)msg.data.getValue()/1000;

			msg = boost::any_cast<messageSensor>(a_environmentData.get(48, DataOption::LAST, in));//
			capteurs[2] += (float)msg.data.getValue()/1000;//

			msg = boost::any_cast<messageSensor>(a_environmentData.get(56, DataOption::LAST, in));
			capteurs[3] += (float)msg.data.getValue()/1000;

			msg = boost::any_cast<messageSensor>(a_environmentData.get(52, DataOption::LAST, in));//
			capteurs[4] += (float)msg.data.getValue()/1000;//

			msg = boost::any_cast<messageSensor>(a_environmentData.get(60, DataOption::LAST, in));
			capteurs[5] += (float)msg.data.getValue()/1000;
			msg = boost::any_cast<messageSensor>(a_environmentData.get(58, DataOption::LAST, in));
			capteurs[6] += (float)msg.data.getValue()/1000;

			//msg = boost::any_cast<messageSensor>(a_environmentData.get(62, DataOption::LAST, in));//
			//capteurs[9] += (float)msg.data.getValue()/1000;//
		}

		// Fait une moyenne
		capteurs[0] /= moyenne;
		capteurs[1] /= moyenne;
		capteurs[2] /= moyenne;
		capteurs[3] /= moyenne;
		capteurs[4] /= moyenne;
		capteurs[5] /= moyenne;
		capteurs[6] /= moyenne;
		capteurs[9] /= moyenne;

		/*for(int id = 0; id < 10; id++)
			_DISPLAY(capteurs[id] << " : ");
		_DISPLAY(std::endl);//*/

		// Recupere l'information de position des codeuses
		messageAsservissement msgA = boost::any_cast<messageAsservissement>(a_environmentData.get(7, DataOption::LAST));

		float *x = new float;
		float *y = new float;
		float *position_angle = new float;

		*x = msgA.y.value/100;			// Conversion en metre
		*y = -1*msgA.x.value/100;		// Convertion en metre
		*position_angle = -1*msgA.alpha.value;	// Changement de referentiel (conferer les specifications)

		Point vitesse;
		floue(capteurs, x, y, position_angle, 0, 0, &vitesse.x, &vitesse.y);	// Planifie
		
		// Mode debug : Affichage de la vitesse stocke
		//_DISPLAY(10*vitesse.x << " | " << 10*vitesse.y << std::endl);

		a_trajectory.push_back(vitesse);	// Stocke la trajectoire
	}
	catch(std::exception & e)
	{
		//_DEBUG(e.what(), WARNING);
	}
}
