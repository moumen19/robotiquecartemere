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
			Buffer(int size = 1024);	// Constructeur
			~Buffer();			// Destructeur

			void put(char);			// Ajoute un octet au buffer
			char get();			// Lit un octet du buffer
			int dataAvailable();		// Retourne le nombre d'octet disponible en lecture

		private: 
			int a_size;			// Taille du buffer circulaire
			int a_readCursor;		// Curseur de lecture
			int a_writeCursor;		// Curseur d'écriture
			char * a_buffer;		// Tableau d'octet : le buffer
	};

#endif

