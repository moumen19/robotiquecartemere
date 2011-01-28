/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Planning.hpp
 *
 */

#ifndef DEF_PLANNING
#define DEF_PLANNING

	#include <vector>

	#include "../Debug.hpp"

	#include "Data.hpp"
	#include "Constraint.hpp"
	#include "Strategy.hpp"
	#include "Sensors.hpp"

	/**
 	 * Structure point/vitesse
	 */
	struct Point
	{
		float x;
		float y;
	};

	/**
  	 * Classe de planification
 	 */
	class Planning
	{
		public:
			Planning(Data &, Constraint &, Strategy &, Sensors &);	// Constructeur
			~Planning();						// Destructeur

			void clearTrajectory();					// Efface les points de la trajectoire
			Point get();						// Retourne le dernier point stocke

			void run();						// Execute la planification

		protected:			
			void flou();						// Planification flou
			void track();						// Suivi d'objet a la camera (rotation)
			void test();

		private:
			Data & a_environmentData;				// Module de stockage des infos d'environnement
			Constraint & a_constraint;				// Module de stockage des contraintes
			Strategy & a_strategy;					// Module de strategie
			Sensors & a_sensors;					// Module des capteurs

			std::vector<Point> a_trajectory;			// Trajectoire de point ou de vitesse
			//Strat a_lastStrategy;					// Dernière strategie 
	};

#endif

