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
	//std::cout << std::endl;
	while(this->a_thread_active && i > -1)
	{
		//messageAsservissement msg;
		char msg;
		if(this->a_asservissement.isDataAvailable())
		{
			msg = boost::any_cast<char>(this->a_asservissement.getData());
			if(i >= 1 && i <=4)
				t.data[i-1] = msg;
			if(i >= 5 && i <=8)
				t.data[i-5] = msg;
			if(i >= 9 && i <=12)
				t.data[i-9] = msg;

			if(i == 4 || i == 8 || i == 12)
			{
				_DISPLAY((float)t.value);
				_DISPLAY(" : ");
			}
			if(i == 0 || i == 13)
			{
				_DISPLAY(msg);
				_DISPLAY(" : ");
			}

			i++;
			if(i == 14)
			{
				i = 0;
				j++;
				_DISPLAY(std::endl);
				_DISPLAY(j);
				_DISPLAY("\t");
			}

			//msg = boost::any_cast<messageAsservissement>(this->a_asservissement.getData());
			//std::cout << std::endl << (int)msg.id << "  : " ;
			/*for(int i = 0; i < 4; i++)    
				std::cout << msg.x.data[i] << "  : " ;
			for(int i = 0; i < 4; i++)    
				std::cout << msg.y.data[i] << "  : " ;
			for(int i = 0; i < 4; i++)    
				std::cout << msg.alpha.data[i] << "  : ";*/
			//std::cout << msg.x.value << "  : " << msg.y.value << "  : " << msg.alpha.value << "  : ";
			//std::cout << (int)msg.commande << std::endl;
		}
		//sleep(1);

		/*if(This->a_rs232Asservissement.IsOpen())
		{
			while(This->a_rs232Asservissement.rdbuf()->in_avail())
			{
				This->a_rs232Asservissement >> m;
				if(m == 42)
				{
					sprintf(s, "Reception de donnees de la part de l'asservissement : %s", This->a_bufferAsservissement);
					_DEBUG(s, INFORMATION);
					This->a_bufferAsservissementCursor = 0;
				}

				if(This->a_bufferAsservissementCursor <= _BUFFER_ASSERVISSEMENT_SIZE)
				{
					This->a_bufferAsservissement[This->a_bufferAsservissementCursor] = m;
					This->a_bufferAsservissementCursor++;
				}
				else
					_DEBUG("Debordement du buffer de reception de donnees asservissement...", WARNING);
			}
		}
		else
			_DEBUG("Erreur de communication avec l'asservissement", WARNING);
		*/

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

	//std::string msg(m);
	//std::cout << msg << std::endl;
	
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

