/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier RS232Asservissement.cpp
 *
 */

#include "RS232Asservissement.hpp"

#include <boost/interprocess/sync/scoped_lock.hpp>

const int RS232Asservissement::messageSize = 14;

RS232Asservissement::RS232Asservissement(std::string port) :
	BusRS232(port)

{

}

/**
 * Destructeur
 */
RS232Asservissement::~RS232Asservissement()
{
	_DEBUG("Destruction du module RS232Asservissement", INFORMATION);
}

/**
 * Methode virtuelle qui formate le msg en tableau de char
 * @param msg - la donnee a envoyer (le type doit être obligatoirement messageAsservissement)
 * @return Retourne le tableau de char sous la forme d'un DataBuffer de la bibliotheque LibSerial (un std::vector de char)
 */
SerialPort::DataBuffer RS232Asservissement::onSend(const boost::any & msg)
{
	messageAsservissement ass_msg = boost::any_cast<messageAsservissement>(msg);	// Le msg doit être un messageAsservissement ici
	SerialPort::DataBuffer buffer;

	// On converti le messageAsservissement en std::vector<char>
	buffer.push_back(ass_msg.id);			// On ajoute l'id
	for(unsigned int i = 0; i < 4; i++)		// On ajoute la position en x
		buffer.push_back(ass_msg.x.data[i]);
	for(unsigned int i = 0; i < 4; i++)		// On ajoute la position en y
		buffer.push_back(ass_msg.y.data[i]);
	for(unsigned int i = 0; i < 4; i++)		// On ajoute l'angle
		buffer.push_back(ass_msg.alpha.data[i]);
	buffer.push_back(ass_msg.commande);		// On ajoute la commande

	return buffer;
}

/**
 * Methode virtuelle qui formate les donnees du buffer circulaire en donnee
 * @return La donnee recu (le type doit être obligatoirement messageAsservissement)
 * @see getData()
 * @see isDataAvailable()
 */
boost::any RS232Asservissement::onReceive()
{
	if(!this->isDataAvailable())
		_DEBUG("Pas de donnée disponible...", WARNING);	// A remplacer par une vraie excpetion

	//boost::interprocess::scoped_lock<boost::mutex> lock(a_mutex, boost::interprocess::try_to_lock);
	this->a_mutex.lock();				// On protege les donnees (a_buffer, a_bufferWriteCursor, a_bufferReadCursor)
	
	messageAsservissement ass_msg;
	this->a_buffer >> ass_msg.id;			// Recuperation de l'identifiant
	for(int i = 0; i < 4; i++)			// Recuperation de la position en x
		this->a_buffer >> ass_msg.x.data[i];
	for(int i = 0; i < 4; i++)			// Recuperation de la position en y
		this->a_buffer >> ass_msg.y.data[i];
	for(int i = 0; i < 4; i++)			// Recuperation de l'angle
		this->a_buffer >> ass_msg.alpha.data[i];
	this->a_buffer >> ass_msg.commande;		// Recuperation de la commande

	this->a_mutex.unlock();				// On deverouille le mutex
	
	boost::any msg = ass_msg;

	return msg;					// retourne un messaage asservissement
}

/**
 * Methode virtuelle qui teste si une donnée est présente dans le buffer circulaire 
 * Cette methode teste si un messageAsservissement est present
 * @return true si la donnee est disponible, false sinon
 * @see onReceive()
 */
bool RS232Asservissement::isDataAvailable()
{
	//boost::interprocess::scoped_lock<boost::mutex> lock(a_mutex, boost::interprocess::try_to_lock);
	this->a_mutex.lock();					// On protege les donnees (a_buffer)
	int bufferAvailable = this->a_buffer.dataAvailable();	// On calcul le nombre d'octets non lu
	this->a_mutex.unlock();					// On deverouille le mutex	

	if(bufferAvailable >= RS232Asservissement::messageSize)
	{
		this->a_mutex.lock();		// On protege le mutex
		while(this->a_buffer.see() != 42 && bufferAvailable > 0)
		{
			unsigned char c;
			this->a_buffer >> c;			
			//_DISPLAY("|" << (int)c << "|");
			bufferAvailable--;
		}
		this->a_mutex.unlock();		// On deverouille le mutex

		if(bufferAvailable >= RS232Asservissement::messageSize)
			return true;
	}

	return false;
}

