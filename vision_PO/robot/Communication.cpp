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
#include <fstream>
#include <time.h>

using namespace std;


/**
 * Constructeur
 * @param sensors - Module des capteurs
 * @param sensorsData - Module des données capteurs
 * @param environment - Module de données environnement
 * @param strategy - Module de strategie
 * @param planning - Module de planification
 */
Communication::Communication(Sensors & sensors, Data & sensorsData, Data & environment, Strategy & strategy, Planning & planning) :
	a_sensors(sensors),
	a_sensorsData(sensorsData),
	a_environmentData(environment),
	a_strategy(strategy),
	a_planning(planning),
	a_RS232Asservissement("/dev/ttyUSB0"),
	a_RS232Sensor(sensors, "/dev/ttyUSB1")
{
	this->a_RS232Asservissement.open();
	this->a_RS232Sensor.open();

	this->a_thread_active = false;

	_DEBUG("Initialisation du module de communication", INFORMATION);
}
/**
 * Destructeur
 */
Communication::~Communication()
{
	this->stop();
	_DEBUG("Destruction du module de communication", INFORMATION);
}

/**
 * Methode envoyant un message à un peripherique (seul l'asservissement est implemente)
 * @param port - le nom du peripherique
 * @param vg - vitesse gauche
 * @param vd -  vitesse droite
 * @param a - angle
 * @param c - commande
 */
void Communication::send(Port::Port port, int vg, int vd, int a, int c)
{
	if(port == Port::ASSERVISSEMENT)
	{		
		messageAsservissement msg;
		msg.id = 42;
		msg.x.value = vg;
		msg.y.value = vd;
		msg.alpha.value = a;
		msg.commande = c;
		this->a_RS232Asservissement.send(msg);
	}
	else if(port == Port::SENSOR)
	{
		
	}
	else
		_DEBUG("Envoie des données à un port non existant !", WARNING);
}

/**
 * Test si le thread est lance
 * @return true si le thread est lance, false sinon
 */
bool Communication::isActive()
{
	return this->a_thread_active;
}

/**
 * Demarre le thread
 */
void Communication::start()
{
	this->a_thread_active = true;
	a_thread = new boost::thread(&Communication::run, this);
}

/**
 * Arrete le thread
 */
void Communication::stop()
{
	this->a_thread_active = false;
}

/**
 * Methode execute par le thread
 * Cette methode recupere et traite les differents messages provenant des peripheriques
 */
void Communication::run()
{
	_DEBUG("Debut de la routine d'ecoute des ports de communications", INFORMATION);
	bool stop_BAU = false;
	bool stop_obs = false;


	ofstream fichier("sensors_data.txt", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
 
        if(!fichier)
                cerr << "Impossible d'ouvrir le fichier !" << endl;


	// Tant qu'on a pas arrete le thread
	while(this->a_thread_active)
	{
		// Recuperation du message asservissement
		if(this->a_RS232Asservissement.isDataAvailable())
		{
			try
			{
				messageAsservissement msg = boost::any_cast<messageAsservissement>(this->a_RS232Asservissement.getData());
				
				_DISPLAY((int)msg.id << " : ");
				/*
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.x.data[i] << " : ");
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.y.data[i] << " : ");

				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.alpha.data[i] << " : ");
				//*/
				_DISPLAY(msg.x.value << " : " << msg.y.value << " : " << msg.alpha.value << " : ");
				_DISPLAY((int)msg.commande << std::endl);

				// Traitement du message				
				switch(msg.commande)
				{
					case 7:
						a_sensorsData.set((int)msg.commande, msg);
						break;
					default:
						_DEBUG("Le message asservissement n'a pas pu etre traite, la commande ne correspond a aucune action repertoriee", WARNING);
				}

			}
			catch(std::exception & e)
			{
				_DEBUG(e.what(), WARNING);
			}

		}

		// Recuperation du message capteur
		if(this->a_RS232Sensor.isDataAvailable())
		{
			try
			{
				boost::any msg_boost = this->a_RS232Sensor.getData();
				messageSensor msg = boost::any_cast<messageSensor>(msg_boost);
				
				// Stockage du message capteur dans le module de stockage de donnee capteur
				a_sensorsData.set((int)msg.id_sensor, msg);

				if(fichier)
				{
					struct timeval tim;
					gettimeofday(&tim, NULL);
					if(msg.id_sensor != 144 && msg.id_sensor != 62)
						fichier << (int)msg.id_sensor << ";" << (float)msg.data.getValue() << ";" << tim.tv_usec <<  std::endl;
				}

				_DISPLAY((int)msg.id << " : ");
				_DISPLAY((int)msg.id_sensor << " : ");
				/*
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.time.getData(i) << " : ");
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.data.getData(i) << " : ");
				for(int i = 0; i < 4; i++)
					_DISPLAY((int)msg.crc.getData(i) << " : ");
				//*/
				_DISPLAY(msg.time.getValue() << " : " << msg.data.getValue() << " : " << msg.crc.getValue());
				_DISPLAY(std::endl);
				

				// Traitement du message capteur
				messageAsservissement msgSend;
				switch(msg.id_sensor)
				{
					// Capteur BAU
					case 144:
						if(msg.data.getValue() == 0)
						{
							// Desactivation de l'arret d'urgence
							if(stop_BAU == true && stop_obs == false)
							{
								a_strategy.set(BAU_OFF);
								msgSend.id = 42;
								msgSend.x.value = 0;
								msgSend.y.value = 0;
								msgSend.alpha.value = 0;
								msgSend.commande = 9;
								this->a_RS232Asservissement.send(msgSend);
								_DISPLAY("Message asservissement : tout est OK [ENVOYE]"<<std::endl);
							}
							stop_BAU = false;
						}
						else
						{
							// Activation de l'arret d'urgence
							if(stop_BAU == false)
							{
								a_strategy.set(BAU_ON);
								msgSend.id = 42;
								msgSend.x.value = 0;
								msgSend.y.value = 0;
								msgSend.alpha.value = 0;
								msgSend.commande = 8;
								this->a_RS232Asservissement.send(msgSend);							
								_DISPLAY("Message asservissement : ARRET [ENVOYE]"<<std::endl);
							}
							stop_BAU = true;
						}
						// Pingage des codeuse à chaque fois que l'on reçoit le message BAU
						msgSend.id = 42;
						msgSend.x.value = 0;
						msgSend.y.value = 0;
						msgSend.alpha.value = 0;
						msgSend.commande = 7;
						this->a_RS232Asservissement.send(msgSend);
						break;
					// Arret d'urgence capteurs
					case 48:
					case 50:
					case 52:
					case 54:
					case 56:
					case 58:
					case 60:
					case 62:
						/*if(msg.data.getValue() <= 150 && msg.id_sensor == 50)	// Si un obstacle est trop proche d'un capteur, il entre en arret d'urgence
						{
							if(stop_obs == false)
							{
								messageAsservissement msgSend;
								msgSend.id = 42;
								msgSend.x.value = 0;
								msgSend.y.value = 0;
								msgSend.alpha.value = 0;
								msgSend.commande = 8;
								this->a_RS232Asservissement.send(msgSend);
								_DEBUG("STOP D'URGENCE !!! Obstacle trop proche", INFORMATION);
							}
							stop_obs = true;
						}
						else
						{
							if(stop_BAU == false && stop_obs == true)
							{
								messageAsservissement msgSend;
								msgSend.id = 42;
								msgSend.x.value = 5;
								msgSend.y.value = 5;
								msgSend.alpha.value = 0;
								msgSend.commande = 9;
								this->a_RS232Asservissement.send(msgSend);
								_DEBUG("Arret d'urgence desactive", INFORMATION);
								stop_obs = false;
							}
						}*/
						break;
					default:
						break;
						//_DEBUG("Le message capteur n'a pas pu etre traite...", WARNING);
				}
			}
			catch(std::exception & e)
			{
				_DEBUG(e.what(), WARNING);
			}
		}	    	
	}

	if(fichier)	
                fichier.close();
	this->a_RS232Asservissement.close();
	this->a_RS232Sensor.close();
	_DEBUG("Fin de la routine d'ecoute des ports de communications", INFORMATION);
}

/*void Communication::test(int i)
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
			/*msg.id = 42;
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

/*	this->a_RS232Asservissement.send(msg);
}*/

