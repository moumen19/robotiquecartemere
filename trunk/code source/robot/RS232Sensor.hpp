/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier RS232Sensor.hpp
 *
 */

#ifndef DEF_RS232SENSOR
#define DEF_RS232SENSOR

	#include "../Debug.hpp"

	#include "BusRS232.hpp"
	#include "Integer.hpp"
	#include "Sensors.hpp"

	/**
	 * La donnee message capteur contenant toutes les infos provenant d'un message asservissement
	 */
	typedef struct
	{
		unsigned char id;
		unsigned char id_sensor;
		Integer time;
		Integer data;
		Integer crc;
	} messageSensor;

	/**
	 * Classe de dialogue avec l'asservissement par bus RS232
	 * @see BusRS232
	 */
	class RS232Sensor : public BusRS232
	{
		public:
			RS232Sensor(Sensors &);			// Constructeur
			virtual ~RS232Sensor();			// Destructeur virtuel
			virtual bool isDataAvailable();		// Teste si une donnée provenant des capteurs est présente dans le buffer circulaire 

		protected:
			virtual SerialPort::DataBuffer onSend(const boost::any &);	// Methode appele a l'appel de send() (transforme la donnee capteur en tableau de char)
			virtual boost::any onReceive();					// Methode appele a l'appel de getData() (transforme les octets du buffer en donnee capteur)

		private:
			Sensors & a_sensors;
	};

#endif

