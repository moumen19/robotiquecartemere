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

Buffer::Buffer(int size)
{
	this->a_size = size;
	this->a_buffer = new char[this->a_size]; 

	
	this->a_readCursor = 0;
	this->a_writeCursor = 0;
}

Buffer::~Buffer()
{
	delete[] this->a_buffer;
}

void Buffer::put(char c)
{
	this->a_buffer[this->a_writeCursor] = c;
	this->a_writeCursor++;

	if(this->a_writeCursor >= this->a_size)
		a_writeCursor = 0;

	if(this->a_writeCursor == this->a_readCursor)
	{
		this->a_readCursor++;
		if(this->a_readCursor >= this->a_size)
			a_readCursor = 0;
	}
}

char Buffer::get()
{
	if(this->dataAvailable())
	{
		char c = this->a_buffer[this->a_readCursor];
		this->a_readCursor++;

		if(this->a_readCursor >= this->a_size)
			this->a_readCursor = 0;

		return c;
	}

		_DEBUG("Pas de donnée disponible...", WARNING);	// A remplacer par une vraie excpetion !!!
	return 0;
}

int Buffer::dataAvailable()
{
	int available = this->a_writeCursor - this->a_readCursor;

	if(this->a_writeCursor < this->a_readCursor)
		available += this->a_size;
	return available;
}
