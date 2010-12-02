/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier BusRS232.cpp
 *
 */

#include "BusRS232.hpp"
#include <string.h>

/**
 * Constructeur indiquant le port COM a ouvrir ainsi que la taille du buffer circulaire
 */
BusRS232::BusRS232(std::string port, int bufferSize)
{
	this->a_port = port;
	this->a_bufferSize = bufferSize;
	this->a_buffer = new char[this->a_bufferSize];
	this->a_bufferWriteCursor = 0;
	this->a_bufferReadCursor = 0;
	this->a_thread_active = false;

	this->a_rs232 = new SerialPort(port);

	_DEBUG("Initialisation du module de communication par bus RS232", INFORMATION);
}

/**
 * Destructeur
 */
BusRS232::~BusRS232()
{
	this->close();
}

/**
 * Ouvre la connexion
 */
bool BusRS232::open()
{
	this->close();

	// Ouvre la connexion
	try
	{
		this->a_rs232->Open(SerialPort::BAUD_9600, 
				    SerialPort::CHAR_SIZE_8, 
				    SerialPort::PARITY_ODD, 
				    SerialPort::STOP_BITS_1, 
				    SerialPort::FLOW_CONTROL_NONE);
	}
	catch(const std::exception & e)
	{
		_DEBUG(e.what(), WARNING);
	}

	// Lance le thread d'ecoute
	if(this->a_rs232->IsOpen())
	{
		_DEBUG("Ouverture du port RS232", INFORMATION);

		this->a_thread_active = true;
		a_thread = new boost::thread(&BusRS232::receive, this);        

		return true;
	}

	_DEBUG("Echec de l'ouverture du port RS232", WARNING);
	return false;
}

/**
 * Ferme la connexion
 */
void BusRS232::close()
{
	// Si la connexion est ouverte, on la ferme
	if(this->a_rs232->IsOpen())
	{
		try
		{   
			this->a_thread_active = false;     
			this->a_rs232->Close();
			//this->a_thread->join()	// Attention blockage ici !
		}
		catch(const std::exception & e)
		{	
		    _DEBUG(e.what(), WARNING);
		}

		_DEBUG("Fermerure du port RS232", INFORMATION);
	}
}


/**
 * Envoi une donnée au port COM
 */
void BusRS232::send(boost::any msg)
{
	SerialPort::DataBuffer buffer = this->onSend(msg);
	this->a_rs232->Write(buffer);
}

/**
 * Formate le msg en tableau de char
 */
SerialPort::DataBuffer BusRS232::onSend(const boost::any & msg)
{
	std::string str_msg = boost::any_cast<std::string>(msg);	// Le msg doit être un std::string ici
	SerialPort::DataBuffer buffer;

	// On converti le std::string en std::vector<char>
	for(unsigned int i = 0; i < str_msg.size(); i++)
		buffer.push_back(str_msg[i]);

	return buffer;
}

/**
 * Méthode thread qui écoute en permanance le port COM et qui stocke chaque octet reçu dans un buffer circulaire
 */
void BusRS232::receive()
{
	_DEBUG("Debut de la routine d'ecoute d'un port RS232", INFORMATION);

	// Tant que l'on a pas ferme la connexion
	while(this->a_thread_active)
	{
		if(this->a_rs232->IsOpen())
		{
			unsigned char buffer = 0;
			try
			{
				// On recupere un octet (timeout = 0 : processus bloquant)
				buffer = this->a_rs232->ReadByte(0);

				a_mutex.lock();		// On protege les donnees (a_buffer, a_bufferWriteCursor, a_bufferReadCursor)

				this->a_buffer[this->a_bufferWriteCursor] = buffer;	// On stocke l'octet recu dans le buffer circulaire
				this->a_bufferWriteCursor++;				// On incremente le curseur d'ecriture
				if(this->a_bufferWriteCursor >= this->a_bufferSize)	// Si le curseur pointe vers un element externe au tableau
					this->a_bufferWriteCursor -= this->a_bufferSize;// On reboucle (position 0)

				if(this->a_bufferWriteCursor == this->a_bufferReadCursor)	// Si le curseur d'ecriture a un tour d'avance par rapport au curseur de lecture
				{
					this->a_bufferReadCursor++;				// On deplace le curseur de lecture de 1 (perte d'information)
					if(this->a_bufferReadCursor >= this->a_bufferSize)	// Si le curseur pointe vers un element externe au tableau
						this->a_bufferReadCursor -= this->a_bufferSize;	// On reboucle (position 0)
				}

				a_mutex.unlock();	// On deverouille le mutex
			}
			catch(const std::exception & e)
			{
				//_DEBUG(e.what(), WARNING);
			}
		} 
		else
			_DEBUG("Port RS232 non ouvert", WARNING);
	}

	_DEBUG("Fin de la routine d'ecoute d'un port RS232", INFORMATION);
}

/**
 * Retourne une donnée reçue (réception d'un char)
 */
boost::any BusRS232::getData()
{
	return onReceive();
}

/**
 * Teste si une donnée est présente dans le buffer circulaire 
 */
bool BusRS232::isDataAvailable()
{
	a_mutex.lock();	// On protege les donnees (a_buffer, a_bufferWriteCursor, a_bufferReadCursor)

	// On calcul le nombre d'octets non lu
	int bufferAvailable = this->a_bufferWriteCursor - this->a_bufferReadCursor;
	if(this->a_bufferWriteCursor < this->a_bufferReadCursor)
	bufferAvailable += this->a_bufferSize;

	a_mutex.unlock();	// On deverouille le mutex	

	if(bufferAvailable > 0)
		return true;

	return false;
}

/**
 * Formate les donnees du buffer circulaire en donnee
 */
boost::any BusRS232::onReceive()
{
	if(!this->isDataAvailable())
		_DEBUG("Pas de donnée disponible...", WARNING);	// A remplacer par une vraie excpetion

	a_mutex.lock();	// On protege les donnees (a_buffer, a_bufferWriteCursor, a_bufferReadCursor)
	
	// On recupere un octet
	boost::any msg = this->a_buffer[this->a_bufferReadCursor];
	this->a_bufferReadCursor++;

	a_mutex.unlock();	// On deverouille le mutex
	
	return msg;	// retourne un char
}

