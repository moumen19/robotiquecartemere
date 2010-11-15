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


//*************************************************************************************************

    // Activation du mode debug
    #define _DEBUG_MODE true

//*************************************************************************************************


    // D�finition d'une macro ajoutant un message
    #define _DEBUG(title, message, priority)    if(_DEBUG_MODE == true) \
                                                    _DEBUG::addMessage(title, message, priority);

    // D�finition d'une macro ex�cutant une m�thode statique
    #define _DEBUG_EXEC(f)  if(_DEBUG_MODE == true) \
                                _DEBUG::f;

    // D�finition des constantes de configurations
    typedef int Debug_Configuration;
    #define CONSOLE_DISPLAY 1
    #define FILE_DISPLAY 2

    // D�finition des constantes de priorit�s de messages
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
        Debug_Priority priority;    // Priorit� du message
        bool isEmpty;               // Validit� du message
    } typedef Message;

    // Classe de d�buggage
    class _DEBUG
    {
        public:
            static void addMessage(std::string, std::string, Debug_Priority);
            static void clearAllMessage();
            static void configuration(Debug_Configuration, std::string);
            static Message getMessage(unsigned int);
            static Message getNextMessage();
            static int getNumMessages();

        private:
            static std::vector<Message> a_messages;
            static unsigned int a_cursor;
            static Debug_Configuration a_configuration;
            static std::string a_filename;
    };

#endif
