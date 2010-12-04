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

	#include "Data.hpp"
	#include "Planning.hpp"
	#include "Constraint.hpp"
	#include "Strategy.hpp"
	#include "DataFusion.hpp"
	#include "Communication.hpp"

	class Robot
	{
		public:
			Robot();
			~Robot();

			void interruptuion_arret_urgence();

			bool isCommunicationActive();
			void startCommunication();
			void stopCommunication();

			// Gestion du thread
			bool isActive();
			void start();
			void stop();
			void wait();

			void test();

		protected:
			void run();

		private:
			Data a_sensorsData;
			Data a_environmentData;
			Constraint a_constraint;
			DataFusion a_dataFusion;
			Strategy a_strategy;
			Planning a_planning;
			Communication a_communication;

			boost::thread *a_thread;
			bool a_thread_active;
	};

	enum Sensor
	{
		US1 = 0,
		US2 = 1,
		US3 = 2,
		US4 = 3, 
		US5 = 5
	};

#endif

