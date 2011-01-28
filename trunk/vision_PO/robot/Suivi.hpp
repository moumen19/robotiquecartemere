/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Suivi.hpp
 *
 */

// Non utilise actuellement
#ifndef DEF_ROBOT
#define DEF_ROBOT

	#include "../Debug.hpp"

	#include "Data.hpp"
	#include "Planning.hpp"
	#include "Communication.hpp"
	

	/**
 	 * 
	 */
	class Suivi
	{
		public:
			Suivi();		// Constructeur
			~Robot();								// Destructeur

		private:
			Sensors & a_sensors;							// Liste des capteurs
			Data & a_sensorsData;							// Module de stockage des infos capteurs
			Planning & a_planning;							// Module de planification de trajectoire
			Communication & a_communication;						// Module de communication avec les capteurs et l'asservissement
	};

#endif

