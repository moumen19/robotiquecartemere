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

	#include "Sensors.hpp"
	#include "Data.hpp"
	#include "Planning.hpp"
	#include "RS232Asservissement.hpp"
	//#include "BusRS232.hpp"	
	#include "RS232Sensor.hpp"

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
			Communication(Sensors &, Data &, Data &, Constraint &, Planning &);
			~Communication();

			void send(Port::Port, std::string);
			
			// Gestion du thread
			bool isActive();
			void start();
			void stop();

			void test(int i = 0);

		protected:
			void run();

		private:
			Sensors & a_sensors;
			Data & a_sensorsData;
			Data & a_environmentData;
			Constraint & a_constraint;
			Planning & a_planning;

			RS232Asservissement a_RS232Asservissement; //RS232Asservissement
			RS232Sensor a_RS232Sensor;

			boost::thread *a_thread;
			bool a_thread_active;
	};

#endif

