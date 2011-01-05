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

	enum Strat
	{
		NONE,
		BAU_ON,
		BAU_OFF,
		GO_AHEAD
	};

	class Strategy
	{
		public:
			Strategy(Data &, Constraint &);
			~Strategy();

			void init(Strat);
			Strat get();
			void set(Strat);
	
			void bau_on();
			void bau_off();
			void go_ahead();

		private:
			Constraint & a_constraint;
			Data & a_environmentData;
			Strat a_strategy;
			Strat a_strategySave;
	};

#endif

