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
 * @param port - Le nom du port COM à ouvrir (/dev/ttyUSB0 par defaut)
 * @param bufferSize - La taille du buffer circulaire (1024 par defaut)
 */
BusRS232::BusRS232(std::string port, int bufferSize) :
	a_buffer(bufferSize),
	a_rs232(port)
{
	this->a_port = port;
	this->a_thread_active = false;

	_DEBUG("Initialisation du module de communication par bus RS232", INFORMATION);
}

/**
 * Destructeur
 */
BusRS232::~BusRS232()
{
	this->close();
	_DEBUG("Destruction du module BusRS232", INFORMATION);
}

/**
 * Ouvre la connexion
 * @return true si la connexion est etablie, false sinon
 */
bool BusRS232::open()
{
    if(a_rs232.IsOpen())
    this->close();	// Si une connexion est deja ouverte, on la ferme

	// Ouvre la connexion
	try
	{
		this->a_rs232.Open(SerialPort::BAUD_9600,
				    SerialPort::CHAR_SIZE_8,
				    SerialPort::PARITY_NONE,
				    SerialPort::STOP_BITS_1,
				    SerialPort::FLOW_CONTROL_NONE);
	}
	catch(const std::exception & e)
	{
		_DEBUG(e.what(), WARNING);
	}

	// Lance le thread d'ecoute
	if(this->a_rs232.IsOpen())
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
	if(this->a_rs232.IsOpen())
	{
		try
		{
			this->a_thread_active = false;
			this->a_rs232.Close();
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
 * @param msg - La donnee à envoyer (par defaut, ce parametre doit etre un SerialPort::DataBuffer. Pour envoyer un autre type de donnee, il faudra creer une classe heritant de BusRS232 et reimplementer onSend())
 * @see onSend()
 */
void BusRS232::send(boost::any msg)
{
	SerialPort::DataBuffer buffer = this->onSend(msg);
	try
	{
		this->a_rs232.Write(buffer);
	}
	catch(std::exception & e)
	{
		_DEBUG(e.what(), WARNING);
	}
}

/**
 * Methode virtuelle qui formate le msg en tableau de char
 * @param msg - la donnee a envoyer (par defaut, ce parametre doit etre un SerialPort::DataBuffer. Pour envoyer un autre type de donnee, il faudra creer une classe heritant de BusRS232 et reimplementer onSend())
 * @return Retourne le tableau de char sous la forme d'un DataBuffer de la bibliotheque LibSerial (un std::vector de char)
 */
SerialPort::DataBuffer BusRS232::onSend(const boost::any & msg)
{
	SerialPort::DataBuffer buffer;
	try
	{
		buffer = boost::any_cast<SerialPort::DataBuffer>(msg);
	}
	catch(std::exception e)
	{
		_DEBUG(e.what(), WARNING);
	}

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
		if(this->a_rs232.IsOpen())
		{
			unsigned char buffer = 0;
			try
			{
				if(this->a_rs232.IsDataAvailable())
				{
					buffer = this->a_rs232.ReadByte(0);	// On recupere un octet (timeout = 0 : processus bloquant)

					a_mutex.lock();				// On protege les donnees (a_buffer, a_bufferWriteCursor, a_bufferReadCursor)
					this->a_buffer << buffer;		// On ajoute un octet au buffer
					a_mutex.unlock();			// On deverouille le mutex
				}
				else
				{}
			}
			catch(const SerialPort::ReadTimeout & e)
			{/* Si on a depasse le timeout, on ne fait rien */}
			catch(const std::exception & e)
			{
				_DEBUG(e.what(), WARNING);
			}
		}
		else
			_DEBUG("Port RS232 non ouvert", WARNING);
	}

	_DEBUG("Fin de la routine d'ecoute d'un port RS232", INFORMATION);
}

/**
 * Retourne une donnée reçue (réception d'un char)
 * @return La donnee recu (par defaut, ce parametre est un char. Pour recuperer un autre type de donnee, il faudra creer une classe heritant de BusRS232 et reimplementer onReceive())
 * @see onReceive()
 */
boost::any BusRS232::getData()
{
	return onReceive();
}

/**
 * Methode virtuelle qui formate les donnees du buffer circulaire en donnee
 * @return La donnee recu (par defaut, ce parametre est un char. Pour recuperer un autre type de donnee, il faudra creer une classe heritant de BusRS232 et reimplementer onReceive())
 * @see getData()
 * @see isDataAvailable()
 */
boost::any BusRS232::onReceive()
{
	if(!this->isDataAvailable())
		_DEBUG("Pas de donnée disponible...", WARNING);	// A remplacer par une vraie excpetion

	a_mutex.lock();				// On protege les donnees (a_buffer, a_bufferWriteCursor, a_bufferReadCursor)
	unsigned char c;
	this->a_buffer >> c; 			// On recupere un octet
	boost::any msg = c;
	a_mutex.unlock();			// On deverouille le mutex

	return msg;				// retourne un char
}

/**
 * Methode virtuelle qui teste si une donnée est présente dans le buffer circulaire
 * Par defaut, cette methode teste si au moins un octet est present
 * Si vous creez une classe heritant de BusRS232 et reimplementez onReceive(), il faudra adapter cette methode pour qu'elle retourne vrai si votre donnee est bien presente
 * @return true si la donnee est disponible, false sinon
 * @see onReceive()
 */
bool BusRS232::isDataAvailable()
{
	a_mutex.lock();						// On protege les donnees (a_buffer, a_bufferWriteCursor, a_bufferReadCursor)
	int bufferAvailable = a_buffer.dataAvailable();		// On recupere le nombre d'octet non lu
	a_mutex.unlock();					// On deverouille le mutex

	if(bufferAvailable > 0)
		return true;

	return false;
}

