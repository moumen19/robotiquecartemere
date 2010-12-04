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

	struct Point
	{
		double x;
		double y;
	};

	class Planning
	{
		public:
			Planning(Data &, Constraint &, Strategy &);
			~Planning();

			void clearTrajectory();

			void run();

		private:
			Data & a_environmentData;
			Constraint & a_constraint;
			Strategy & a_strategy;
			std::vector<Point> a_trajectory;
	};

#endif

