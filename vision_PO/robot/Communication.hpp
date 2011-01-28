/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Communication.hpp
 *
 */

#ifndef DEF_COMMUNICATION
#define DEF_COMMUNICATION

	#include <string>
	#include <boost/thread/thread.hpp>

	#include "../Debug.hpp"

	#include "Sensors.hpp"
	#include "Data.hpp"
	#include "Strategy.hpp"
	#include "Planning.hpp"
	#include "RS232Asservissement.hpp"
	#include "RS232Sensor.hpp"

	/**
 	 * Enumeration regroupant les noms des peripheriques communiquant avec le programme principal
	 */
	namespace Port
	{
		enum Port
		{
			ASSERVISSEMENT,
			SENSOR
		};
	}

	/**
	 * Cette classe s'occupe de communiquer avec toutes les peripheriques et de traiter les messages provenant de ceux-ci
	 */
	class Communication
	{
		public:
			Communication(Sensors &, Data &, Data &, Strategy &, Planning &);	// Constructeur
			~Communication();							// Destructeur

			void send(Port::Port, int, int, int, int);				// Envoi un message a un peripherique
			
			// Gestion du thread
			bool isActive();				// Test si le thread de la communication est actif
			void start();					// Debute le thread de la partie communication
			void stop();					// Arrete le thread

			//void test(int i = 0);

		protected:
			void run();					// Fonction thread se chargent de récupérer et de traiter les messages des différents peripheriques

		private:
			Sensors & a_sensors;				// Module capteurs
			Data & a_sensorsData;				// Module donnees capteurs
			Data & a_environmentData;			// Module donnees environnement
			Strategy & a_strategy;				// Module strategie
			Planning & a_planning;				// Module plannification

			RS232Asservissement a_RS232Asservissement; 	// Module de communication RS232 avec l'asservissement
			RS232Sensor a_RS232Sensor;			// Module de communication RS232 avec les capteurs

			boost::thread *a_thread;			// Le thread
			bool a_thread_active;				// Un flag indiquant si le thread est actif
	};

#endif

