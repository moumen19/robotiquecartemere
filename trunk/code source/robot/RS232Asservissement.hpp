/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier RS232Asservissement.hpp
 *
 */

#ifndef DEF_RS232ASSERVISSEMENT
#define DEF_RS232ASSERVISSEMENT

	#include "../Debug.hpp"
	#include "BusRS232.hpp"

	/**
	 * Definie une union pour passer directement d'un float à un tableau de char
	 */
	typedef union
	{
		float value;
		unsigned char data[4];
	} decoupFloat;

	/**
	 * La donnee message asservissement contenant toutes les infos provenant d'un message asservissement
	 */
	typedef struct
	{
		unsigned char id;
		decoupFloat x;
		decoupFloat y;
		decoupFloat alpha;
		unsigned char commande;
	} messageAsservissement;

	/**
	 * Classe de dialogue avec l'asservissement par bus RS232
	 * @see BusRS232
	 */
	class RS232Asservissement : public BusRS232
	{
		public:
			virtual ~RS232Asservissement();
			virtual bool isDataAvailable();	// Teste si une donnée provenant de l'asservissement est présente dans le buffer circulaire 
			static const int messageSize;	// Taille du message asservissement

		protected:
			virtual SerialPort::DataBuffer onSend(const boost::any &);	// Methode appele a l'appel de send() (transforme la donnee asservissement en tableau de char)
			virtual boost::any onReceive();					// Methode appele a l'appel de getData() (transforme les octets du buffer en donnee asservissement)
	};

#endif

