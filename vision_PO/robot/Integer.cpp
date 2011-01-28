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

/**
 * Constructeur
 */
Integer::Integer()
{
	a_int.value = 0;
}

/**
 * Detructeur
 */
Integer::~Integer()
{

}

/**
 * Retourne la valeur du nombre
 * @return la valeur de l'unsigned integer
 */
unsigned int Integer::getValue()
{
	return a_int.value;
}

/**
 * Retourne un octet de l'unsigned integer
 * @param i - le numero de l'octet a recuperer
 * @return un octet
 */
unsigned char Integer::getData(int i)
{
	if(i >= 0 && i < 4)
		return a_int.data[i];
	return 0;
}

/**
 * Definie la valeur a stocker
 * @param v - valeur a stocker
 */
void Integer::setValue(unsigned int v)
{
	a_int.value = v;
}

/**
 * Definie la valeur d'un octet
 * @param c - la valeur de l'octet
 * @param i - la position de l'octet
 * @return true si tout s'est bien passe, false sinon
 */
bool Integer::setData(unsigned char c, int i)
{
	if(i < 0 && i >= 4)
		return false;
	a_int.data[i] = c;
	return true;
}

/**
 * Definie la valeur de tous les octets
 * @param c - la tableau de quatre octets
 */
void Integer::setData(unsigned char c[4])
{
	for(int i = 0; i < 4; i++)
		a_int.data[i] = c[i];
}

