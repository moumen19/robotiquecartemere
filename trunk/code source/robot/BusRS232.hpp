/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier BusRS232.hpp
 *
 */

#ifndef DEF_BUSRS232
#define DEF_BUSRS232

	#include <string>
	#include <SerialPort.h>
	#include <boost/any.hpp>
	#include <boost/thread/thread.hpp>
	#include <boost/thread/mutex.hpp>

	#include "../Debug.hpp"

	class BusRS232
	{
		public:
			BusRS232(std::string port = "/dev/ttyUSB0", int bufferSize = 1024);	// Constructeur indiquant le port COM a ouvrir ainsi que la taille du buffer circulaire
			virtual ~BusRS232();	// Destructeur

			bool open();		// Ouvre la connexion
			void close();		// Ferme la connexion

			boost::any getData();	// Retourne une donnée reçue (réception d'un char)	//A caster comme ceci : char monChar = boost::any_cast<char>(monAny);
			void send(boost::any);	// Envoi une donnée au port COM (envoi d'un string)

			virtual bool isDataAvailable();	// Teste si une donnée est présente dans le buffer circulaire (ici, une donnée = un octet (un char))

		protected:
			virtual SerialPort::DataBuffer onSend(const boost::any &);	// Methode appele a l'appel de send() (transforme la donnee en tableau de char)
			virtual boost::any onReceive();					// Methode appele a l'appel de getData() (transforme les octets du buffer en donnee)
			void receive();							// Méthode thread qui écoute en permanance le port COM et qui stocke chaque octet reçu dans un buffer circulaire

		private:
			std::string a_port;		// Nom du port COM
			SerialPort * a_rs232; 		// Le port COM

			boost::thread *a_thread;	// Le thread
			bool a_thread_active;		// Le flag de sorti du thread

			char * a_buffer;		// Le buffer circulaire
			int a_bufferSize;		// La taille du buffer circulaire
			int a_bufferWriteCursor;	// Un cursor vers la case du buffer ou il faut ecrire la donnee
			int a_bufferReadCursor;		// Un cursor vers la case du buffer ou il faut lire la donnee

			boost::mutex a_mutex;		// Un mutex de protection
	};

#endif

