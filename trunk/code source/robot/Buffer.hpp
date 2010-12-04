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
			Buffer(int size = 1024);
			~Buffer();

			void put(char);
			char get();
			int dataAvailable();

		private: 
			int a_size;
			int a_readCursor;
			int a_writeCursor;
			char * a_buffer;
	};

#endif

