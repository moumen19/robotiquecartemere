/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
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

    // Inclusion des bibliothèques
    #include <iostream>
    #include <vector>
    #include <boost/thread/mutex.hpp>
    #include <sstream>


//*************************************************************************************************

    // Activation du mode debug
    #define _DEBUG_MODE true

//*************************************************************************************************


    // Définition d'une macro ajoutant un message
    #define _DEBUG(message, priority)    if(_DEBUG_MODE == true) \
                                                { \
                                                    std::ostringstream oss; \
                                                    oss << __FILE__ << " (" << __LINE__ << ")"; \
                                                    _DEBUG::lock(); \
                                                    _DEBUG::addMessage(oss.str(), message, priority); \
                                                    _DEBUG::unlock(); \
                                                }

    // Définition d'une macro exécutant une méthode statique
    #define _DEBUG_EXEC(f)  if(_DEBUG_MODE == true) \
                                _DEBUG::f;

    // Définition des constantes de configurations
    typedef int Debug_Configuration;
    #define DISPLAY_NONE 0
    #define CONSOLE_DISPLAY 1
    #define FILE_DISPLAY 2

    // Définition des constantes de priorités de messages
    enum Debug_Priority
    {
        FATAL,
        WARNING,
        INFORMATION
    };

    // Structure d'un message
    struct Message
    {
        std::string title;          // Titre du message
        std::string content;        // Contenu du message
        Debug_Priority priority;    // Priorité du message
        bool isEmpty;               // Définie si le message à un sens
    } typedef Message;

    // Classe de débuggage
    class _DEBUG
    {
        public:
            static void addMessage(std::string, std::string, Debug_Priority);	// Ajoute un message
            static void clearAllMessage();					// Efface tous les messages
            static void configuration(Debug_Configuration, std::string);	// Configure le mode Debug
            static Message getMessage(unsigned int);				// Retourne un message
            static Message getNextMessage();					// Retourne le message suivant
            static int getNumMessages();					// Donne le nombre de message stocké
            static void lock();							// Verouille un mutex
            static void unlock();						// Deverouille le mutex

        private:
            static std::vector<Message> a_messages;				// Stockage des messages
            static unsigned int a_cursor;					// Curseur indiquant quel sera le prochain message retourne par getNextMessage()
            static Debug_Configuration a_configuration;				// Configuration du mode Debug
            static std::string a_filename;					// Chemin du fichier de logs si existant
	    static boost::mutex a_mutex;					// Mutex de protection

    };

#endif
