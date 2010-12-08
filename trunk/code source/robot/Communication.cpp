/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Communication.cpp
 *
 */

#include "Communication.hpp"
#include <sstream>

Communication::Communication(Data & sensors, Data & environment, Constraint & constraint, Planning & planning) :
	a_sensorsData(sensors),
	a_environmentData(environment),
	a_constraint(constraint),
	a_planning(planning)
{
	this->a_asservissement.open();

	//this->a_sensor = new BusRS232();
	//this->a_sensor->open();

	this->a_thread_active = false;

	_DEBUG("Initialisation du module de communication", INFORMATION);
}

Communication::~Communication()
{
	this->stop();
}

void Communication::send(Port::Port port, std::string msg)
{
	if(port == Port::ASSERVISSEMENT)
	{
		this->a_asservissement.send(msg);
	}
	else if(port == Port::SENSOR)
	{
		//this->a_sensor->send(msg);
	}
	else
		_DEBUG("Envoie des données à un port non existant !", WARNING);
}

/*void Communication::receive()
{

}*/

bool Communication::isActive()
{
	return this->a_thread_active;
}

void Communication::start()
{
	this->a_thread_active = true;
	a_thread = new boost::thread(&Communication::run, this);
}

void Communication::stop()
{
	this->a_thread_active = false;
}

void Communication::run()
{
	_DEBUG("Debut de la routine d'ecoute des ports de communications", INFORMATION);


	int i = 0, j = 0;
	decoupFloat t;
	//_DISPLAY(std::endl);
	while(this->a_thread_active && i > -1)
	{
		//messageAsservissement msg;
		char msg = 0;
		if(this->a_asservissement.isDataAvailable())
		{

			/*
			try
			{
				msg = boost::any_cast<char>(this->a_asservissement.getData());
			}
			catch(std::exception & e)
			{
				_DEBUG(e.what(), WARNING);
			}
            //*/


			//msg = boost::any_cast<messageAsservissement>(this->a_asservissement.getData());
			//_DISPLAY(std::endl << (int)msg.id << "  : ");
			/*for(int i = 0; i < 4; i++)
				_DISPLAY(msg.x.data[i] << "  : " );
			for(int i = 0; i < 4; i++)
				_DISPLAY(msg.y.data[i] << "  : " );
			for(int i = 0; i < 4; i++)
				_DISPLAY(msg.alpha.data[i] << "  : ");*/
			//_DISPLAY(msg.x.value << "  : " << msg.y.value << "  : " << msg.alpha.value << "  : ");
			//_DISPLAY((int)msg.commande << std::endl);

		}

		/*this->a_rs232.rdbuf()->in_avail();*/

	}
	this->a_asservissement.close();
	_DEBUG("Fin de la routine d'ecoute des ports de communications", INFORMATION);
}

void Communication::test()
{
	decoupFloat x, y, alpha;
	x.value = 5;
	y.value = 5;
	alpha.value = 0;

	SerialPort::DataBuffer m;

	m.push_back(42);

	for(int i = 0; i < 4; i++)
		m.push_back(x.data[i]);

	for(int i = 0; i < 4; i++)
		m.push_back(y.data[i]);

	for(int i = 0; i < 4; i++)
		m.push_back(alpha.data[i]);

	m.push_back(7);

	/*messageAsservissement msg;
	msg.id = 42;
	msg.x.value = 0;
	msg.y.value = 0;
	msg.alpha.value = 0;
	msg.commande = 7;*/
	/*messageAsservissement msg;
	msg.id = 42;
	msg.x.value = 5;
	msg.y.value = 5;
	msg.alpha.value = 0;
	msg.commande = 3;*/

	this->a_asservissement.send(m);
}

