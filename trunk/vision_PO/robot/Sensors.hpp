#ifndef DEF_SENSORS
#define DEF_SENSORS
		
	#include "../Debug.hpp"

	#include <string>
	#include <map>

	/**
 	 * Classe gerant un capteur
 	 */
	class Sensor
	{
		public:
			Sensor();			// Constructeur
			Sensor(int, std::string, int);	// Constructeur
			~Sensor();			// Destructeur

			int getId();			// Retourne l'identifiant du capteur
			int getSize();			// Retourne la taille du message capteur
			std::string getName();		// Retourne le nom du capteur

			void setId(int);		// Definie l'identifiant du capteur
			void setSize(int);		// Definie la taille du message capteur
			void setName(std::string);	// Definir le nom du capteur

		private:
			int a_id;			// Identifiant du capteur
			int a_size;			// Taille du message capteur
			std::string a_name;		// Nom du capteur
	};

	/**
 	 * Classe gerant tous des capteurs
 	 */
	class Sensors
	{
		public:
			Sensors();					// Constructeur
			~Sensors();					// Destructeur

			void addSensor(int, std::string, int);		// Ajoute un capteur
			bool sensorExist(int);				// Teste l'existance d'un capteur
			bool sensorExist(std::string);			// Teste l'existance d'un capteur
			Sensor getSensor(int);				// Retourne un capteur
			Sensor getSensor(std::string);			// Retourne un capteur

		private: 
			std::map<std::string, Sensor *> a_sensorName;	// Map des capteurs par noms
			std::map<int, Sensor *> a_sensorId;		// Map des capteurs par identifiant
	};

#endif

