/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier Debug.cpp
 *
 */

#include "Debug.hpp"

#if(_DEBUG_MODE == true)

    #include <fstream>

    /*Message::Message(std::string  title, std::string  content, Debug_Priority priority) throw()
    {
        a_title = title;
        a_content = content;
        a_priority = priority;

        std::cout << "Envoi d'une exeption" << std::endl;
    }

    Message::~Message() throw()
    {

    }

    const char * Message::what()const throw()
    {
        return a_content.c_str();
    }*/



    // Définition des Attributs statiques
    std::vector<Message> _DEBUG::a_messages;
    unsigned int _DEBUG::a_cursor = 0;
    Debug_Configuration _DEBUG::a_configuration = CONSOLE_DISPLAY;
    std::string _DEBUG::a_filename = "";
    pthread_mutex_t _DEBUG::a_mutex = PTHREAD_MUTEX_INITIALIZER;

    void _DEBUG::addMessage(std::string title, std::string content, Debug_Priority priority)
    {
        Message m;
        m.title = title;
        m.content = content;
        m.priority = priority;
        m.isEmpty = false;
        _DEBUG::a_messages.push_back(m);


        std::string msg;
        switch(m.priority)
        {
            case FATAL:
                msg = "(Fatal error)";
                break;
            case WARNING:
                msg = "(Warning)";
                break;
            case INFORMATION:
                msg = "(Information)";
                break;
            default:
                msg = "(?)";
        }
        msg += "\n\t";
        msg += m.title;
        msg += " :\n\t* ";
        msg += m.content;


        if((_DEBUG::a_configuration & CONSOLE_DISPLAY) == CONSOLE_DISPLAY)
        {
            std::cout << "DEBUG MODE " << msg << std::endl;
        }

        if((_DEBUG::a_configuration & FILE_DISPLAY) == FILE_DISPLAY)
        {
            std::ofstream file(_DEBUG::a_filename.c_str(), std::ios::out | std::ios::app);

            if(file)
            {
                file << msg << std::endl;;
                file.close();
            }
            else
            {
                    std::cerr << "Erreur: Impossible d'ouvrir le fichier de logs du mode DEBUG !" << std::endl;
            }

        }
    }

    void _DEBUG::clearAllMessage()
    {
        _DEBUG::a_cursor = 0;
        _DEBUG::a_messages.clear();
    }

    void _DEBUG::configuration(Debug_Configuration configuration, std::string filename = "")
    {
        _DEBUG::a_configuration = configuration;
        _DEBUG::a_filename = filename;
    }

    Message _DEBUG::getMessage(unsigned int index)
    {
        if(index < _DEBUG::a_messages.size())
            return _DEBUG::a_messages[index];

        Message m;
        m.isEmpty = true;
        return m;
    }

    Message _DEBUG::getNextMessage()
    {
        if(_DEBUG::a_cursor < _DEBUG::a_messages.size())
        {
            _DEBUG::a_cursor++;
            return _DEBUG::a_messages[a_cursor-1];
        }

        Message m;
        m.isEmpty = true;
        return m;
    }

    int _DEBUG::getNumMessages()
    {
        return _DEBUG::a_messages.size();
    }

    void _DEBUG::lock()
    {
         pthread_mutex_lock(&_DEBUG::a_mutex);
    }

    void _DEBUG::unlock()
    {
         pthread_mutex_unlock(&_DEBUG::a_mutex);
    }

#endif
