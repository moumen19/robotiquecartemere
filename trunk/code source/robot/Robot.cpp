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
	a_planning(a_environmentData, a_constraint, a_strategy),
	a_communication(a_sensorsData, a_environmentData, a_constraint, a_planning)
{
	this->a_threadConfiguration = config;
	this->a_thread_active = false;

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
			//this->a_thread->join();
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

	while(this->a_thread_active)
	{
		//_DEBUG("Robot", INFORMATION);
	}

	_DEBUG("Fin de la routine de calcul des trajectoires", INFORMATION);
}

void Robot::test(int i)
{
	this->a_communication.test(i);
}

