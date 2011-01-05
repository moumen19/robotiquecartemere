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
//#include "plan_flou.c"

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
			//this->flou();
			break;
		default:
			_DEBUG("La strategie ne correspond à aucune", WARNING);
	}

}

/*void Planning::flou()
{
	float capteurs[10];
	
	messageSensor msg = boost::any_cast<messageSensor>(a_sensorsData.get(50, DataOption::LAST));
	capteurs[0] = (float)msg.data.getValue();
	messageSensor msg = boost::any_cast<messageSensor>(a_sensorsData.get(54, DataOption::LAST));
	capteurs[1] = (float)msg.data.getValue();
	capteurs[2] = 1.5;
	messageSensor msg = boost::any_cast<messageSensor>(a_sensorsData.get(56, DataOption::LAST));
	capteurs[3] = (float)msg.data.getValue();
	capteurs[4] = 1.5;
	messageSensor msg = boost::any_cast<messageSensor>(a_sensorsData.get(60, DataOption::LAST));
	capteurs[5] = (float)msg.data.getValue();
	messageSensor msg = boost::any_cast<messageSensor>(a_sensorsData.get(58, DataOption::LAST));
	capteurs[6] = (float)msg.data.getValue();
	capteurs[7] = 0;
	capteurs[8] = 0;
	capteurs[9] = 3;

	float *x;
	float *y;
	float *position_angle;
	Point vitesse;
	
	floue(capteurs, x, y, position_angle, 0, 300, &vitesse.x, &vitesse.y)
}*/

