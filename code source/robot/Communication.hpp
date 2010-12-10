/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Communication.hpp
 *
 */

#ifndef DEF_COMMUNICATION
#define DEF_COMMUNICATION

	#include <string>
	#include <boost/thread/thread.hpp>

	#include "../Debug.hpp"

	#include "Data.hpp"
	#include "Planning.hpp"
	#include "RS232Asservissement.hpp"
	#include "BusRS232.hpp"

	namespace Port
	{
		enum Port
		{
			ASSERVISSEMENT,
			SENSOR
		};
	}

	class Communication
	{
		public:
			Communication(Data &, Data &, Constraint &, Planning &);
			~Communication();

			void send(Port::Port, std::string);
			//void receive();

			// Gestion du thread
			bool isActive();
			void start();
			void stop();

			void test(int i = 0);

		protected:
			void run();

		private:
			RS232Asservissement a_asservissement;
			BusRS232 * a_sensor;//RS232Sensor a_sensor;

			Data & a_sensorsData;
			Data & a_environmentData;
			Constraint & a_constraint;
			Planning & a_planning;

			boost::thread *a_thread;
			bool a_thread_active;
	};

#endif

