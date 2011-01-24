/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Robot.cpp
 *
 */

#include "Robot.hpp"
#include <sstream>

/**
 * Constructeur initialisant les différents modules du robot
 * @param config - Permet d'executer la fusion de donnees et de la planification dans un thread a part ou dans le programme principale
 */
Robot::Robot(Robot_Configuration config) :
	a_dataFusion(a_sensorsData, a_environmentData),
	a_strategy(a_environmentData, a_constraint),
	//a_planning(a_environmentData, a_constraint, a_strategy),
	a_planning(a_sensorsData, a_constraint, a_strategy, a_sensors),
	a_communication(a_sensors, a_sensorsData, a_environmentData, a_strategy, a_planning)
{
	this->a_threadConfiguration = config;
	this->a_thread_active = false;

	a_sensors.addSensor(48, "US1", 2);
	a_sensors.addSensor(50, "US2", 2);
	a_sensors.addSensor(52, "US3", 2);
	a_sensors.addSensor(54, "US4", 2);
	a_sensors.addSensor(56, "US5", 2);
	a_sensors.addSensor(58, "US6", 2);
	a_sensors.addSensor(60, "US7", 2);
	a_sensors.addSensor(62, "US8", 2);

	/*a_sensors.addSensor(80, "IR1", 2);	
	a_sensors.addSensor(83, "IR2", 2);
	a_sensors.addSensor(86, "IR3", 2);	
	a_sensors.addSensor(89, "IR4", 2);
		
	a_sensors.addSensor(10, "BO1", 2);
	a_sensors.addSensor(176, "AC1", 4);*/
	a_sensors.addSensor(144, "BAU", 1);
	//a_sensors.addSensor(112, "VID", 1);

	_DEBUG("Initialisation du robot", INFORMATION);
}

/**
 * Destructeur
 */
Robot::~Robot()
{
	if(this->a_communication.isActive())
		this->a_communication.stop();
	this->stop();
	_DEBUG("Destruction du robot", INFORMATION);
}

/**
 * Teste si la communication avec les capteurs est l'asservissement est lance
 * @return true si la communication est lance, false sinon
 */
bool Robot::isCommunicationActive()
{
	return this->a_communication.isActive();
}

/**
 * Lance le module de communication avec les capteurs et l'asservissement
 */
void Robot::startCommunication()
{
	this->a_communication.start();
}

/**
 * Arrete le module de communication avec les capteurs et l'asservissement
 */
void Robot::stopCommunication()
{
	this->a_communication.stop();
}

/**
 * Teste si le robot est fusionne les donnees capteurs et planifie bien la trajectoire
 * @return true si le robot est actif, false sinon
 */
bool Robot::isActive()
{
	return this->a_thread_active;
}

/**
 * Demarre le robot
 * Cette methode lance le module de fusion des donnees capteurs et le module de planification de trajectoire
 */
void Robot::start()
{
	this->a_thread_active = true;
	switch(this->a_threadConfiguration)
	{
		case ACTIVE_MAIN_THREADING:
			a_thread = new boost::thread(&Robot::run, this);
			break;
		case DESACTIVE_MAIN_THREADING:
			this->run();
			break;
		default:
			_DEBUG("Impossible de lancer le robot", WARNING);
	}

}

/**
 * Arrete le robot
 * Stope le module de fusion des donnees capteurs et le module de planification de trajectoire
 */
void Robot::stop()
{
	this->a_thread_active = false;
}

/**
 * Attend jusqu'a l'arret du robot
 */
void Robot::wait()
{
	switch(this->a_threadConfiguration)
	{
		case ACTIVE_MAIN_THREADING:
			this->a_thread->join();
			break;
		default:
			_DEBUG("La methode wait n'a aucun effet car le threading principale est desactive", WARNING);
	}
}

/**
 * Thread faisant tourner le module de fusion de donnee et de planification de trajectoire
 */
void Robot::run()
{
	_DEBUG("Debut de la routine de calcul des trajectoires", INFORMATION);

	a_strategy.init(GO_AHEAD);

	a_rendererSensor = new display;
	
	while(this->a_thread_active)
	{
		try
		{
			//*
			messageSensor msg = boost::any_cast<messageSensor>(a_sensorsData.get(48, DataOption::LAST));
			this->a_rendererSensor->setSensorDistance((int)msg.id_sensor, (float)msg.data.getValue());

			msg = boost::any_cast<messageSensor>(a_sensorsData.get(50, DataOption::LAST));
			this->a_rendererSensor->setSensorDistance((int)msg.id_sensor, (float)msg.data.getValue());
			
			msg = boost::any_cast<messageSensor>(a_sensorsData.get(52, DataOption::LAST));
			this->a_rendererSensor->setSensorDistance((int)msg.id_sensor, (float)msg.data.getValue());

			msg = boost::any_cast<messageSensor>(a_sensorsData.get(54, DataOption::LAST));
			this->a_rendererSensor->setSensorDistance((int)msg.id_sensor, (float)msg.data.getValue());

			msg = boost::any_cast<messageSensor>(a_sensorsData.get(56, DataOption::LAST));
			this->a_rendererSensor->setSensorDistance((int)msg.id_sensor, (float)msg.data.getValue());

			msg = boost::any_cast<messageSensor>(a_sensorsData.get(58, DataOption::LAST));
			this->a_rendererSensor->setSensorDistance((int)msg.id_sensor, (float)msg.data.getValue());

			msg = boost::any_cast<messageSensor>(a_sensorsData.get(60, DataOption::LAST));
			this->a_rendererSensor->setSensorDistance((int)msg.id_sensor, (float)msg.data.getValue());

			msg = boost::any_cast<messageSensor>(a_sensorsData.get(62, DataOption::LAST));
			this->a_rendererSensor->setSensorDistance((int)msg.id_sensor, (float)msg.data.getValue());

			msg = boost::any_cast<messageSensor>(a_sensorsData.get(144, DataOption::LAST));
			this->a_rendererSensor->setSensorDistance((int)msg.id_sensor, (float)msg.data.getValue());//*/
		}
		catch(std::exception & e)
		{
			//_DEBUG(e.what(), WARNING);
		}

		this->a_planning.run();		// Lance l'algo de planification

		try
		{		
			Point p = a_planning.get();
			a_communication.send(Port::ASSERVISSEMENT, 10*p.x, 10*p.y, 0, 4);	// Envoi a l'asservissement les commandes de deplacement roue gauche/droite
		}
		catch(std::exception & e)
		{
		}
	}

	delete a_rendererSensor;
	this->stop();

	_DEBUG("Fin de la routine de calcul des trajectoires", INFORMATION);
}

/*void Robot::test(int i)
{
	this->a_communication.test(i);
}*/

