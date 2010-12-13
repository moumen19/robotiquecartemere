/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Integer.cpp
 *
 */

#include "Integer.hpp"

Integer::Integer()
{
	a_int.value = 0;
}

Integer::~Integer()
{

}

unsigned int Integer::getValue()
{
	return a_int.value;
}

unsigned char Integer::getData(int i)
{
	if(i >= 0 && i < 4)
		return a_int.data[i];
	return 0;
}

void Integer::setValue(unsigned int v)
{
	a_int.value = v;
}

bool Integer::setData(unsigned char c, int i)
{
	if(i < 0 && i >= 4)
		return false;
	a_int.data[i] = c;
	return true;
}

void Integer::setData(unsigned char c[4])
{
	for(int i = 0; i < 4; i++)
		a_int.data[i] = c[i];
}

