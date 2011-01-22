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
#include <math.h>


#include "plan_flou.cc"

Planning::Planning(Data & environment, Constraint & constraint, Strategy & strategy, Sensors & sensors) :
	a_environmentData(environment),
	a_constraint(constraint),
	a_strategy(strategy),
	a_sensors(sensors)
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
			this->flou();
			break;
		default:
			_DEBUG("La strategie ne correspond à aucune", WARNING);
	}

}

Point Planning::get()
{
	if(a_trajectory.size() == 0)
		throw std::out_of_range("ERREUR : Aucune trajectoire stocke... !");

	return a_trajectory.at(a_trajectory.size()-1);
}

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
	capteurs[9] = 0;
	try
	{
		for(int in = 0; in < 1; in++)
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

		capteurs[0] /= 1;
		capteurs[1] /= 1;
		capteurs[2] /= 1;
		capteurs[3] /= 1;
		capteurs[4] /= 1;
		capteurs[5] /= 1;
		capteurs[6] /= 1;
		capteurs[9] /= 1;

		/*for(int id = 0; id < 10; id++)
			_DISPLAY(capteurs[id] << " : ");
		_DISPLAY(std::endl);//*/
		messageAsservissement msgA = boost::any_cast<messageAsservissement>(a_environmentData.get(7, DataOption::LAST));

		float *x = new float;
		float *y = new float;
		float *position_angle = new float;

		*x = msgA.y.value;
		*y = -1*msgA.x.value;
		*position_angle = -1*msgA.alpha.value;

		Point vitesse;
	
		floue(capteurs, x, y, position_angle, 3, 0, &vitesse.x, &vitesse.y);
		
		//_DISPLAY(vitesse.x << " | " << vitesse.y);

		a_trajectory.push_back(vitesse);
	}
	catch(std::exception & e)
	{
		//_DEBUG(e.what(), WARNING);
	}
}

