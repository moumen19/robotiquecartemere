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
        a_messages.push_back(m);


        std::string msg;
        switch(m.priority)
        {
            case FATAL:
                msg = "(Fatal error) | ";
                break;
            case WARNING:
                msg = "(Warning) | ";
                break;
            case INFORMATION:
                msg = "(Information) | ";
                break;
            default:
                msg = "(?) | ";
        }
        msg += m.title;
        msg += " :\t";
        msg += m.content;


        if((a_configuration & CONSOLE_DISPLAY) == CONSOLE_DISPLAY)
        {
            std::cout << "DEBUG MODE " << msg << std::endl;
        }

        if((a_configuration & FILE_DISPLAY) == FILE_DISPLAY)
        {
            std::ofstream file(a_filename.c_str(), std::ios::out | std::ios::app);

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
        a_cursor = 0;
        a_messages.clear();
    }

    void _DEBUG::configuration(Debug_Configuration configuration, std::string filename = "")
    {
        a_configuration = configuration;
        a_filename = filename;
    }

    Message _DEBUG::getMessage(unsigned int index)
    {
        if(index < a_messages.size())
            return a_messages[index];

        Message m;
        m.isEmpty = true;
        return m;
    }

    Message _DEBUG::getNextMessage()
    {
        if(a_cursor < a_messages.size())
        {
            a_cursor++;
            return a_messages[a_cursor-1];
        }

        Message m;
        m.isEmpty = true;
        return m;
    }

    int _DEBUG::getNumMessages()
    {
        return a_messages.size();
    }

    void _DEBUG::lock()
    {
         pthread_mutex_lock(&a_mutex);
    }

    void _DEBUG::unlock()
    {
         pthread_mutex_unlock(&a_mutex);
    }

#endif
