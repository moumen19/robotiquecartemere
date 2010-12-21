#ifndef DEF_SENSORS
#define DEF_SENSORS
		
	#include "../Debug.hpp"

	#include <string>
	#include <map>

	class Sensor
	{
		public:
			Sensor();
			Sensor(int, std::string, int);
			~Sensor();

			int getId();
			int getSize();
			std::string getName();

			void setId(int);
			void setSize(int);
			void setName(std::string);

		private:
			int a_id;
			int a_size;
			std::string a_name;
	};

	class Sensors
	{
		public:
			Sensors();
			~Sensors();

			void addSensor(int, std::string, int);
			bool sensorExist(int);
			bool sensorExist(std::string);
			Sensor getSensor(int);
			Sensor getSensor(std::string);

		private: 
			std::map<std::string, Sensor *> a_sensorName;
			std::map<int, Sensor *> a_sensorId;
	};

#endif

