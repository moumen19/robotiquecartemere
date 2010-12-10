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
	_DEBUG("Destruction du module Communication", INFORMATION);
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

	while(this->a_thread_active)
	{
		if(this->a_asservissement.isDataAvailable())
		{
			try
			{
				messageAsservissement msg = boost::any_cast<messageAsservissement>(this->a_asservissement.getData());
				
				_DISPLAY((int)msg.id << " : ");
				//*
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.x.data[i] << " : ");
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.y.data[i] << " : ");

				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.alpha.data[i] << " : ");
				//*/
				_DISPLAY(msg.x.value << " : " << msg.y.value << " : " << msg.alpha.value << " : ");
				_DISPLAY((int)msg.commande << std::endl);

				switch(msg.commande)
				{
					case 0:
						break;
					default:
						_DEBUG("Le message asservissement n'a pas pu etre traite, la commande ne correspon a aucune action repertoriee", WARNING);
				}

			}
			catch(std::exception & e)
			{
				_DEBUG(e.what(), WARNING);
			}

		}
	}
	this->a_asservissement.close();
	_DEBUG("Fin de la routine d'ecoute des ports de communications", INFORMATION);
}

void Communication::test(int i)
{
	messageAsservissement msg;
	switch(i)
	{
		case 0:
			msg.id = 42;
			msg.x.value = 0;
			msg.y.value = 0;
			msg.alpha.value = 0;
			msg.commande = 0;
			break;
		case 3:
			msg.id = 42;
			msg.x.value = 5;
			msg.y.value = 5;
			msg.alpha.value = 0;
			msg.commande = 3;
			break;
		case 7:
			msg.id = 42;
			msg.x.value = 0;
			msg.y.value = 0;
			msg.alpha.value = 0;
			msg.commande = 7;
			break;
		default:
			_DEBUG("La commande asservissement n'est pas valide", WARNING);
			return;
	}

	this->a_asservissement.send(msg);
}

