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

	typedef union
	{
		unsigned int value;		// Bien vérifier que ce soit sur 4 octets
		unsigned char data[4];
	} decoupInt;

	class Integer
	{
		public:
			Integer();
			~Integer();

			unsigned int getValue();
			unsigned char getData(int);
			void setValue(unsigned int);
			bool setData(unsigned char, int);
			void setData(unsigned char c[4]);

		//private:
			decoupInt a_int;
	};

#endif

