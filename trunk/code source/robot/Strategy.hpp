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
		BAU_START,
		BAU_STOP,
		GO_AHEAD
	};

	class Strategy
	{
		public:
			Strategy(Data &, Constraint &);
			~Strategy();

			Strat get();
			void set(Strat);
	
			void bau_start();
			void bau_stop();
			void go_ahead();

		private:
			Constraint & a_constraint;
			Data & a_environmentData;
			Strat a_strategy;
			Strat a_strategySave;
	};

#endif

