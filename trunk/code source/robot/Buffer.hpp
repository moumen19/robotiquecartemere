/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Buffer.cpp
 *
 */

#ifndef DEF_BUFFER
#define DEF_BUFFER

	#include "../Debug.hpp"

	/**
	 * La classe Buffer gère un buffer circulaire
	 */
	class Buffer
	{
		public:
			Buffer(int size = 1024);		// Constructeur
			~Buffer();				// Destructeur

			void put(const unsigned char &);	// Ajoute un octet au buffer
			unsigned char get();			// Lit un octet du buffer
			int dataAvailable();			// Retourne le nombre d'octet disponible en lecture

			void operator<<(const unsigned char &);	// Ajoute un octet au buffer
			void operator>>(unsigned char &);	// Lit un octet du buffer

		private: 
			int a_size;				// Taille du buffer circulaire
			int a_readCursor;			// Curseur de lecture
			int a_writeCursor;			// Curseur d'écriture
			unsigned char * a_buffer;		// Tableau d'octet : le buffer
	};

#endif

