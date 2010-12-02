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
    //#include <pthread.h>
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

    /*#define _DEBUG(title, message, priority)    if(_DEBUG_MODE == true) \
                                                { \
                                                    throw Message(title, message, priority); \
                                                }*/

    // Définition d'une macro exécutant une méthode statique
    #define _DEBUG_EXEC(f)  if(_DEBUG_MODE == true) \
                                _DEBUG::f;

    // Définition des constantes de configurations
    typedef int Debug_Configuration;
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
        bool isEmpty;               // Définie si le message à ubn sens
    } typedef Message;

    /*class Message : public std::exception
    {
        public:
            Message(std::string , std::string , Debug_Priority) throw();
            virtual ~Message() throw();
            virtual const char * what()const throw();

        private:
            std::string a_title;          // Titre du message
            std::string a_content;        // Contenu du message
            Debug_Priority a_priority;    // Priorité du message
    };*/

    // Classe de débuggage
    class _DEBUG
    {
        public:
            static void addMessage(std::string, std::string, Debug_Priority);
            static void clearAllMessage();
            static void configuration(Debug_Configuration, std::string);
            static Message getMessage(unsigned int);
            static Message getNextMessage();
            static int getNumMessages();
            static void lock();
            static void unlock();

        private:
            static std::vector<Message> a_messages;
            static unsigned int a_cursor;
            static Debug_Configuration a_configuration;
            static std::string a_filename;
            //static pthread_mutex_t a_mutex;
	    static boost::mutex a_mutex;

    };

#endif
