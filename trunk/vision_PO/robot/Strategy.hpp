/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Strategy.hpp
 *
 */

#ifndef DEF_STRATEGY
#define DEF_STRATEGY

	#include "../Debug.hpp"

	#include "Constraint.hpp"
	#include "Data.hpp"

	/**
 	 * Enumeration des differentes strategies
 	 */
	enum Strat
	{
		NONE,
		BAU_ON,
		BAU_OFF,
		GO_AHEAD
	};

	/**
 	 * Class gerant la strategie en cours
	 */
	class Strategy
	{
		public:
			Strategy(Data &, Constraint &);	// Constructeur
			~Strategy();			// Destructeur

			void init(Strat);		// Initialisation de la strategie
			Strat get();			// Retourne la strategie en cours
			void set(Strat);		// Definie la strategie
	
		protected:
			void bau_on();			// Strategie BAU activee
			void bau_off();			// Strategie BAU relache
			void go_ahead();		// Strategie principale

		private:
			Constraint & a_constraint;	// Module de contraitnes
			Data & a_environmentData;	// Module des donnees d'environnement

			Strat a_strategy;		// Strategie en cours
			Strat a_strategySave;		// Strategie sauvegarder
	};

#endif

