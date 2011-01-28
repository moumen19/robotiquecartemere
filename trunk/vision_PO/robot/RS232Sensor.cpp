/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier RS232Sensor.cpp
 *
 */

#include "RS232Sensor.hpp"

#include <boost/interprocess/sync/scoped_lock.hpp>


/**
 * Constructeur
 */
RS232Sensor::RS232Sensor(Sensors & sensors, std::string port) : 	
	BusRS232(port),
	a_sensors(sensors) 
{

}

/**
 * Destructeur
 */
RS232Sensor::~RS232Sensor()
{
	_DEBUG("Destruction du module RS232Asservissement", INFORMATION);
}

/**
 * Methode virtuelle qui formate le msg en tableau de char
 * @param msg - la donnee a envoyer (le type doit être obligatoirement messageAsservissement)
 * @return Retourne le tableau de char sous la forme d'un DataBuffer de la bibliotheque LibSerial (un std::vector de char)
 */
SerialPort::DataBuffer RS232Sensor::onSend(const boost::any & msg)
{
	messageSensor ass_sensor = boost::any_cast<messageSensor>(msg);	// Le msg doit être un messageAsservissement ici
	SerialPort::DataBuffer buffer;

	// On converti le messageAsservissement en std::vector<char>
	//buffer.push_back(ass_msg.id);			// On ajoute l'id

	_DEBUG("onSend non encore implemente", WARNING)

	return buffer;
}

/**
 * Methode virtuelle qui formate les donnees du buffer circulaire en donnee
 * @return La donnee recu (le type doit être obligatoirement messageAsservissement)
 * @see getData()
 * @see isDataAvailable()
 */
boost::any RS232Sensor::onReceive()
{
	if(!this->isDataAvailable())
		_DEBUG("Pas de donnée disponible...", WARNING);	// A remplacer par une vraie excpetion

	//boost::interprocess::scoped_lock<boost::mutex> lock(a_mutex, boost::interprocess::try_to_lock);
	this->a_mutex.lock();				// On protege les donnees (a_buffer, a_bufferWriteCursor, a_bufferReadCursor)
	
	messageSensor sensor_msg;
	this->a_buffer >> sensor_msg.id;			// Recuperation de l'identifiant
	this->a_buffer >> sensor_msg.id_sensor;			// Recuperation de l'identifiant capteur

	sensor_msg.time.setData(this->a_buffer.get(), 0);	// Recuperation du temps
	sensor_msg.time.setData(this->a_buffer.get(), 1);
	
	for(int i = 0; i < a_sensors.getSensor(sensor_msg.id_sensor).getSize(); i++)	// Recuperation de la donnee
		sensor_msg.data.setData(this->a_buffer.get(), i);

	sensor_msg.crc.setData(this->a_buffer.get(), 0);	// Recuperation du crc
	sensor_msg.crc.setData(this->a_buffer.get(), 1);
	

	this->a_mutex.unlock();				// On deverouille le mutex
	
	boost::any msg = sensor_msg;

	return msg;					// retourne un messaage asservissement
}

/**
 * Methode virtuelle qui teste si une donnée est présente dans le buffer circulaire 
 * Cette methode teste si un messageAsservissement est present
 * @return true si la donnee est disponible, false sinon
 * @see onReceive()
 */
bool RS232Sensor::isDataAvailable()
{
	//boost::interprocess::scoped_lock<boost::mutex> lock(a_mutex, boost::interprocess::try_to_lock);
	this->a_mutex.lock();					// On protege les donnees (a_buffer)
	int bufferAvailable = this->a_buffer.dataAvailable();	// On calcul le nombre d'octets non lu
	this->a_mutex.unlock();					// On deverouille le mutex	

	if(bufferAvailable >= 7)
	{
		this->a_mutex.lock();		// On protege le mutex
		while(this->a_buffer.see() != 17 && bufferAvailable > 0)
		{
			unsigned char c;
			this->a_buffer >> c;			
			//_DISPLAY("i" << (int)c << "|");
			bufferAvailable--;
		}
		this->a_mutex.unlock();		// On deverouille le mutex

		if(bufferAvailable >= 7)
		{
			unsigned char sensorID = this->a_buffer.see(1);
			if(a_sensors.sensorExist((unsigned int)sensorID))
			{
				Sensor s = a_sensors.getSensor(sensorID);
				if(bufferAvailable >= s.getSize() + 6)
					return true;
			}
			else
			{
				//_DISPLAY(std::endl << sensorID << std::endl);
				//_DEBUG("Le message n'appartient a aucun capteur connu", WARNING);
				unsigned char c;
				this->a_buffer >> c;			
				//_DISPLAY((unsigned int)sensorID << "c" << (int)c << "|");
				bufferAvailable--;
			}
		}
	}

	return false;
}

