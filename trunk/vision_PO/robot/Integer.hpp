/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Integer.hpp
 *
 */

#ifndef DEF_INTEGER
#define DEF_INTEGER

	#include "../Debug.hpp"

	/**
	 * Union pour découper un integer en octet
	 */
	typedef union
	{
		unsigned int value;		// Bien vérifier que ce soit sur 4 octets
		unsigned char data[4];
	} decoupInt;

	/**
 	 * Classe manipulant un unsigned integer au niveau de l'octet
	 */
	class Integer
	{
		public:
			Integer();				// Constructeur
			~Integer();				// Destructeur

			unsigned int getValue();		// Retourne la valeur de l'integer
			unsigned char getData(int);		// Retourne un octet de la valeur de l'integer
			void setValue(unsigned int);		// Stocke une valeur
			bool setData(unsigned char, int);	// Stocke un octet
			void setData(unsigned char c[4]);	// Stocke le tableau d'octet definissant l'integer

		private:
			decoupInt a_int;			// Le nombre en question
	};

#endif

