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

	class Strategy
	{
		public:
			Strategy(Data &, Constraint &);
			~Strategy();

			int get();
			void set(int);

		private:
			Constraint & a_constraint;
			Data & a_environmentData;
			int a_strategy;
	};

#endif

