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

	struct Point
	{
		float x;
		float y;
	};

	class Planning
	{
		public:
			Planning(Data &, Constraint &, Strategy &, Sensors &);
			~Planning();

			void clearTrajectory();
			void flou();

			Point get();

			void run();

		private:
			Data & a_environmentData;
			Constraint & a_constraint;
			Strategy & a_strategy;
			Sensors & a_sensors;
			std::vector<Point> a_trajectory;
			Strat a_lastStrategy;
	};

#endif

