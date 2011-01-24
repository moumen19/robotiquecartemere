/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Robot.hpp
 *
 */

#ifndef DEF_ROBOT
#define DEF_ROBOT

	#include <boost/thread/thread.hpp>

	#include "../Debug.hpp"
	#include "../display.h"

	#include "Data.hpp"
	#include "Planning.hpp"
	#include "Constraint.hpp"
	#include "Strategy.hpp"
	#include "DataFusion.hpp"
	#include "Communication.hpp"
	#include "Sensors.hpp"

	/**
 	 * Enumeration de configuration du thread principal du robot robot
	 */
	enum Robot_Configuration
	{
		ACTIVE_MAIN_THREADING,		// Cree un thread a part pour le robot
		DESACTIVE_MAIN_THREADING	// Utilise le thread principal pour le robot
	};

	/**
 	 * La classe robot permet de gerer et de coordonnee l'ensemble des actions des differents modules du robot
	 */
	class Robot
	{
		public:
			Robot(Robot_Configuration config = ACTIVE_MAIN_THREADING);		// Constructeur
			~Robot();								// Destructeur

			bool isCommunicationActive();						// Teste si le module de communication est lance
			void startCommunication();						// Lance le dialogue avec les capteurs et l'asservissement
			void stopCommunication();						// Arrete le dialogue avec les capteurs et l'asservissement

			// Gestion du thread
			bool isActive();							// Teste si le robot est actif
			void start();								// Lance le traitement des donnees capteurs et la planification de trajectoire
			void stop();								// Desactive le robot
			void wait();								// Attend jusqu'a ce que le robot soit desactive

			//void test(int i = 0);//////////

		protected:
			void run();								// Thread traitant les donnees capteurs et planifiant la trajectoire

		private:
			Sensors a_sensors;							// Liste des capteurs
			Data a_sensorsData;							// Module de stockage des infos capteurs
			Data a_environmentData;							// Module de stockage des infos haut niveau sur l'environement (perception du robot)
			Constraint a_constraint;						// Module specifiant les contraintes du robot
			DataFusion a_dataFusion;						// Module de traitement des infos capteurs pour les transformer en infos environement
			Strategy a_strategy;							// Module décidant la strategie que le robot doit adopter
			Planning a_planning;							// Module de planification de trajectoire
			Communication a_communication;						// Module de communication avec les capteurs et l'asservissement

			boost::thread *a_thread;						// Le thread
			bool a_thread_active;							// Un flag d'arret du thread
			Robot_Configuration a_threadConfiguration;				// Configuration du multi threading

			display * a_rendererSensor;						// Affichage graphique des capteurs
	};

#endif

