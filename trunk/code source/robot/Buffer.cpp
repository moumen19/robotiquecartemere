/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Buffer.hpp
 *
 */

#include "Buffer.hpp"

/** 
 * Constructeur
 * @param size - La taille du buffer circulaire
 */
Buffer::Buffer(int size)
{
	this->a_size = size;
	this->a_buffer = new unsigned char[this->a_size]; 
	
	this->a_readCursor = 0;
	this->a_writeCursor = 0;
}

/**
 * Destructeur
 */
Buffer::~Buffer()
{
	delete[] this->a_buffer;
	_DEBUG("Destruction du module Buffer", INFORMATION);
}

/**
 * Ajoute un octet au buffer
 * @param c - L'octet a ajouter au buffer
 */
void Buffer::put(const unsigned char &c)
{
	this->a_buffer[this->a_writeCursor] = c;	// On ajoute l'octet au buffer
	this->a_writeCursor++;				// On incremente le curseur d'ecriture

	if(this->a_writeCursor >= this->a_size)		// Si le curseur sort du buffer
		a_writeCursor = 0;			// On remet le curseur d'ecriture a 0

	if(this->a_writeCursor == this->a_readCursor)	// Si le curseur d'ecriture croise le curseur de lecture
	{
		this->a_readCursor++;			// On incremente le curseur de lecture (perte de donnees)
		if(this->a_readCursor >= this->a_size)	// Si le curseur de lecture sort du buffer
			a_readCursor = 0;		// On remet a O le curseur de lecture
	}
}

/**
 * Recupere le premier octet non lu
 * @return Le premeir octet non lu
 */
unsigned char Buffer::get()
{
	if(this->dataAvailable())	// Si on a des donnees a recuperer
	{
		unsigned char c = this->a_buffer[this->a_readCursor];	// On recupere le premier caractere
		this->a_readCursor++;					// On incremente le curseur de lecture

		if(this->a_readCursor >= this->a_size)			// Si le curseur de lecture sort du buffer
			this->a_readCursor = 0;				// On remet a 0 le curseur de lecture

		return c;
	}

	_DEBUG("Pas de donnée disponible...", WARNING);			// A remplacer par une vraie excpetion !!!
	return 0;
}

/**
 * Regarde l'octet i sans retirer celui-ci du buffer
 * @param i - le numero de l'octet a lire a partir du curseur de lecture (0 par defaut)
 * @return L'octet i du buffer a partir du curseur de lecture
 */
unsigned char Buffer::see(int i)
{
	unsigned char c = this->a_buffer[(this->a_readCursor+i)%this->a_size];	// On regarde le caractere numéro i à partir du curseur de lecture
	return c;
}

/**
 * Retourne le nombre d'octet disponible à la lecture
 * @return Le nombre d'octet disponible
 */
int Buffer::dataAvailable()
{
	int available = this->a_writeCursor - this->a_readCursor;	// On calcul le nombre d'octet disponible

	if(this->a_writeCursor < this->a_readCursor)			
		available += this->a_size;				
	return available;
}

/**
 * Surcharge d'operateur de flux
 * Ajoute un octet au buffer
 */
void Buffer::operator<<(const unsigned char & c)
{
	this->put(c);
}

/**
 * Surcharge d'operateur de flux
 * Recupere un octet du buffer
 */
void Buffer::operator>>(unsigned char & c)
{
	c = this->get();
}

