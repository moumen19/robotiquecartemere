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

const int RS232Asservissement::messageSize = 14;

/**
 * Destructeur
 */
RS232Asservissement::~RS232Asservissement()
{

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
	buffer.push_back(ass_msg.id);
	for(unsigned int i = 0; i < 4; i++)
		buffer.push_back(ass_msg.x.data[i]);
	for(unsigned int i = 0; i < 4; i++)
		buffer.push_back(ass_msg.y.data[i]);
	for(unsigned int i = 0; i < 4; i++)
		buffer.push_back(ass_msg.alpha.data[i]);
	buffer.push_back(ass_msg.commande);

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

	a_mutex.lock();			// On protege les donnees (a_buffer, a_bufferWriteCursor, a_bufferReadCursor)
	
	// On recupere un octet
	messageAsservissement ass_msg;

	ass_msg.id = this->a_buffer[this->a_bufferReadCursor];
	this->a_bufferReadCursor++;		// On repositionne le curseur de lecture
	for(int i = 0; i < 4; i++)
	{
		ass_msg.x.data[i] = this->a_buffer[this->a_bufferReadCursor];
		this->a_bufferReadCursor++;	// On repositionne le curseur de lecture
	}
	for(int i = 0; i < 4; i++)
	{
		ass_msg.y.data[i] = this->a_buffer[this->a_bufferReadCursor];
		this->a_bufferReadCursor++;	// On repositionne le curseur de lecture
	}
	for(int i = 0; i < 4; i++)
	{
		ass_msg.alpha.data[i] = this->a_buffer[this->a_bufferReadCursor];
		this->a_bufferReadCursor++;	// On repositionne le curseur de lecture
	}
	ass_msg.commande = this->a_buffer[this->a_bufferReadCursor];
	this->a_bufferReadCursor++;		// On repositionne le curseur de lecture
	
	boost::any msg = ass_msg;

	a_mutex.unlock();			// On deverouille le mutex
	
	return msg;				// retourne un char
}

/**
 * Methode virtuelle qui teste si une donnée est présente dans le buffer circulaire 
 * Cette methode teste si un messageAsservissement est present
 * @return true si la donnee est disponible, false sinon
 * @see onReceive()
 */
bool RS232Asservissement::isDataAvailable()
{
	a_mutex.lock();	// On protege les donnees (a_buffer, a_bufferWriteCursor, a_bufferReadCursor)

	// On calcul le nombre d'octets non lu
	int bufferAvailable = this->a_bufferWriteCursor - this->a_bufferReadCursor;
	if(this->a_bufferWriteCursor < this->a_bufferReadCursor)
	bufferAvailable += this->a_bufferSize;

	a_mutex.unlock();	// On deverouille le mutex	


	if(bufferAvailable >= 14)//this->messageSize)
	{
		/*if(this->a_buffer[this->a_bufferReadCursor] != 42)
		{
			this->a_bufferReadCursor += 14;//;this->messageSize;
		}*/
		return true;
	}

	return false;
}
