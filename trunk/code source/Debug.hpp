/*
 *
 * Bureau d'�tude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Debug.hpp
 *
 */

#ifndef DEF_DEBUG
#define DEF_DEBUG

	// Inclusion des biblioth�ques
	#include <iostream>
	#include <vector>
	#include <boost/thread/mutex.hpp>
	#include <sstream>


//*************************************************************************************************

	// Activation du mode debug
	#define _DEBUG_MODE true

//*************************************************************************************************


	/**
	 * D�finition d'une macro ajoutant un message
	 * Cette macro d�finie le titre automatiquement (fichier + ligne)
	 * Les donn�es sont automatiquement prot�g�s ce qui permet de lancer la macro dans plusieurs threads en m�me temps
	 * @param message - Le message de Debug
	 * @param priority - Le niveau de priorit� du message
	 * @see Debug_Priority
	 * @see addMessage()
 	 */
	#define _DEBUG(message, priority)	if(_DEBUG_MODE == true) \
						{ \
						    std::ostringstream oss; \
						    oss << __FILE__ << " (" << __LINE__ << ")"; \
						    _DEBUG::lock(); \
						    _DEBUG::addMessage(oss.str(), message, priority); \
						    _DEBUG::unlock(); \
						}

	/** 
	 * D�finition d'une macro ex�cutant une m�thode statique publique
	 * @param f - La m�thode statique avec ses parametres
	 */
	#define _DEBUG_EXEC(f)	if(_DEBUG_MODE == true) \
		                _DEBUG::f;

	/** 
	 * D�finition des constantes de configurations
	 */
	typedef int Debug_Configuration;
	#define DISPLAY_NONE 0		// N'affiche aucun message
	#define CONSOLE_DISPLAY 1	// Affiche les messages dans la console
	#define FILE_DISPLAY 2		// Affiche les messages dans un fichier
	#define SAVE_IN_MEMORY 4	// Enregistre les messages en memoire

	/**
	 * D�finition des constantes de priorit�s de messages
	 * @see _DEBUG()
	 * @see addMessage()
	 */
	enum Debug_Priority
	{
		FATAL,
		WARNING,
		INFORMATION
	};

	/** 
	 * Structure d'un message
	 */
	struct Message
	{
		std::string title;          // Titre du message
		std::string content;        // Contenu du message
		Debug_Priority priority;    // Priorit� du message
		bool isEmpty;               // D�finie si le message � un sens
	} typedef Message;

	/**
	 * Classe de d�buggage
	 */
	class _DEBUG
	{
		public:
			static void addMessage(std::string, std::string, Debug_Priority);	// Ajoute un message
			static void clearAllMessage();						// Efface tous les messages
			static void configuration(Debug_Configuration, std::string);		// Configure le mode Debug
			static Message getMessage(unsigned int);				// Retourne un message
			static Message getNextMessage();					// Retourne le message suivant
			static int getNumMessages();						// Donne le nombre de message stock�
			static void lock();							// Verouille un mutex
			static void unlock();							// Deverouille le mutex

		private:
			static std::vector<Message> a_messages;					// Stockage des messages
			static unsigned int a_cursor;						// Curseur indiquant quel sera le prochain message retourne par getNextMessage()
			static Debug_Configuration a_configuration;				// Configuration du mode Debug
			static std::string a_filename;						// Chemin du fichier de logs si existant
			static boost::mutex a_mutex;						// Mutex de protection
	};

#endif

