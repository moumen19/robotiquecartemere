/*
 *
 * Bureau d'�tude Robotique M2 ISEN 2010-2011
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

Communication::Communication(Sensors & sensors, Data & sensorsData, Data & environment, Strategy & strategy) :
	a_sensors(sensors),
	a_sensorsData(sensorsData),
	a_environmentData(environment),
	a_strategy(strategy),
	a_RS232Asservissement("/dev/ttyUSB0"),
	a_RS232Sensor(sensors, "/dev/ttyUSB1")
{
	this->a_RS232Asservissement.open();
	this->a_RS232Sensor.open();

	this->a_thread_active = false;

	_DEBUG("Initialisation du module de communication", INFORMATION);
}

Communication::~Communication()
{
	this->stop();
	_DEBUG("Destruction du module de communication", INFORMATION);
}

void Communication::send(Port::Port port, std::string msg)
{
	if(port == Port::ASSERVISSEMENT)
	{
		this->a_RS232Asservissement.send(msg);
	}
	else if(port == Port::SENSOR)
	{
		//this->a_RS232Sensor->send(msg);
	}
	else
		_DEBUG("Envoie des donn�es � un port non existant !", WARNING);
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
		if(this->a_RS232Asservissement.isDataAvailable())
		{
			try
			{
				messageAsservissement msg = boost::any_cast<messageAsservissement>(this->a_RS232Asservissement.getData());
				
				//_DISPLAY((int)msg.id << " : ");
				/*
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.x.data[i] << " : ");
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.y.data[i] << " : ");

				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.alpha.data[i] << " : ");
				//*/
				//_DISPLAY(msg.x.value << " : " << msg.y.value << " : " << msg.alpha.value << " : ");
				//_DISPLAY((int)msg.commande << std::endl);

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

		if(this->a_RS232Sensor.isDataAvailable())
		{
			try
			{
				boost::any msg_boost = this->a_RS232Sensor.getData();
				messageSensor msg = boost::any_cast<messageSensor>(msg_boost);
				
				a_sensorsData.set((int)msg.id_sensor, msg);

if(true && (int)msg.id_sensor == 144)
{
				//_DISPLAY((int)msg.id << " : ");
				//_DISPLAY((int)msg.id_sensor << " : ");
				/*
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.time.getData(i) << " : ");
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.data.getData(i) << " : ");
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.crc.getData(i) << " : ");
				//*/
				//_DISPLAY(msg.time.getValue() << " : " << msg.data.getValue() << " : " << msg.crc.getValue());
				//_DISPLAY(std::endl);

				/*
				switch(msg.id_sensor)
				{
					case 144:
						if(msg.data.getValue() == 0)
							a_strategy.set(BAU_STOP);
						else
							a_strategy.set(BAU_START);
						break;
					default:
						_DEBUG("Le message capteur n'a pas pu etre traite...", WARNING);
				}
				//*/
}
			}
			catch(std::exception & e)
			{
				_DEBUG(e.what(), WARNING);
			}

		}

		
		
		 	//a_renderer.setSensorDistance(0x30, 50);
		 	//sleep(2);
	    	
	}
	this->a_RS232Asservissement.close();
	this->a_RS232Sensor.close();
	_DEBUG("Fin de la routine d'ecoute des ports de communications", INFORMATION);
}

void Communication::test(int i)
{
	//*
	messageAsservissement msg;
	int vg = 5, vd = 5;
	switch(i)
	{
		case 3:
			msg.id = 42;
			msg.x.value = 0;
			msg.y.value = 0;
			msg.alpha.value = 0;
			msg.commande = 3;
			break;
		case 4:
			/*
			std::cout << "Vitesse gauche : ";
			std::cin >> vg;
			std::cout << "Vitesse droite : ";
			std::cin >> vd;//*/
			msg.id = 42;
			msg.x.value = vg;
			msg.y.value = vd;
			msg.alpha.value = 0;
			msg.commande = 4;
			break;
		case 7:
			msg.id = 42;
			msg.x.value = 0;
			msg.y.value = 0;
			msg.alpha.value = 0;
			msg.commande = 7;
			break;
		case 8:
			msg.id = 42;
			msg.x.value = 0;
			msg.y.value = 0;
			msg.alpha.value = 0;
			msg.commande = 8;
			break;
		case 9:
			msg.id = 42;
			msg.x.value = 0;
			msg.y.value = 0;
			msg.alpha.value = 0;
			msg.commande = 9;
			break;
		default:
			_DEBUG("La commande asservissement n'est pas valide", WARNING);
			return;
	}//*/

	/*
	SerialPort::DataBuffer msg;
	msg.push_back(42);

	msg.push_back(5);
	msg.push_back(0);
	msg.push_back(0);
	msg.push_back(0);

	msg.push_back(5);
	msg.push_back(0);
	msg.push_back(0);
	msg.push_back(0);

	msg.push_back(0);
	msg.push_back(0);
	msg.push_back(0);
	msg.push_back(0);

	msg.push_back(4);//*/

	this->a_RS232Asservissement.send(msg);
}

