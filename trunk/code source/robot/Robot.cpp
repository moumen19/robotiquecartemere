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

Robot::Robot() :
	a_dataFusion(a_sensorsData, a_environmentData),
	a_strategy(a_environmentData, a_constraint),
	a_planning(a_environmentData, a_constraint, a_strategy),
	a_communication(a_sensorsData, a_environmentData, a_constraint, a_planning)
{
	this->a_thread_active = false;

	_DEBUG("Initialisation du robot", INFORMATION);
}

Robot::~Robot()
{
	if(this->a_communication.isActive())
		this->a_communication.stop();
	this->stop();
}

bool Robot::isCommunicationActive()
{
	return this->a_communication.isActive();
}

void Robot::startCommunication()
{
	this->a_communication.start();
}

void Robot::stopCommunication()
{
	this->a_communication.stop();
}

bool Robot::isActive()
{
	return this->a_thread_active;
}

void Robot::start()
{
	this->a_thread_active = true;
	a_thread = new boost::thread(&Robot::run, this);
}

void Robot::stop()
{
	this->a_thread_active = false;
}

void Robot::wait()
{
	this->a_thread->join();
}

void Robot::run()
{
	_DEBUG("Debut de la routine de calcul des trajectoires", INFORMATION);

	//double donnee = 20.01;
	//this->a_sensorsData->set(0, donnee);
	//donnee = 10.2;
	//this->a_sensorsData->set(0, donnee);
	//std::cout << boost::any_cast<double>(this->a_sensorsData->get(0, DataOption::LAST)) << std::endl;
	//std::cout << boost::any_cast<double>(this->a_sensorsData->get(0, DataOption::FIRST)) << std::endl;

	while(this->a_thread_active)
	{
		//_DEBUG("Robot", INFORMATION);
	}

	_DEBUG("Fin de la routine de calcul des trajectoires", INFORMATION);
}

void Robot::test()
{
	this->a_communication.test();
}

