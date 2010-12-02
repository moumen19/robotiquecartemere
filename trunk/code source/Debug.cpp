/*
 *
 * Bureau d'�tude Robotique M2 ISEN 2010-2011
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



	// D�finition des Attributs statiques
	std::vector<Message> _DEBUG::a_messages;
	unsigned int _DEBUG::a_cursor = 0;
	Debug_Configuration _DEBUG::a_configuration = CONSOLE_DISPLAY;
	std::string _DEBUG::a_filename = "";
	boost::mutex _DEBUG::a_mutex;

	/**
	 * Ajoute un message en m�moire.
	 * Affiche le message dans la console et/ou dans un fichier de logs selon la configuration
	 */
	void _DEBUG::addMessage(std::string title, std::string content, Debug_Priority priority)
	{
		// Cr�ation du message
		Message m;
		m.title = title;
		m.content = content;
		m.priority = priority;
		m.isEmpty = false;
		_DEBUG::a_messages.push_back(m);	// Stockage du message

		if(_DEBUG::a_configuration != DISPLAY_NONE)
		{
			// Mise en forme du message
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

			// Affichage en console
			if((_DEBUG::a_configuration & CONSOLE_DISPLAY) == CONSOLE_DISPLAY)
			{
			    std::cout << "DEBUG MODE " << msg << std::endl;
			}

			// Ecriture dans un fichier de logs
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
	}

	/**
	 * Efface tous les messages de la m�moire
 	 */	
	void _DEBUG::clearAllMessage()
	{
		_DEBUG::a_cursor = 0;
		_DEBUG::a_messages.clear();
	}

	/**
 	 * Configure le mode de Deboggage
	 */
	void _DEBUG::configuration(Debug_Configuration configuration, std::string filename = "")
	{
		_DEBUG::a_configuration = configuration;
		_DEBUG::a_filename = filename;
	}

	/**
	 * Retourne le message numero index
	 */
	Message _DEBUG::getMessage(unsigned int index)
	{
		if(index < _DEBUG::a_messages.size())
		    return _DEBUG::a_messages[index];

		Message m;
		m.isEmpty = true;
		return m;
	}

	/**
	 * Retourne le message non lu le plus ancien
	 */
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

	/**
	 * Retourne le nombre de messages stock�s en m�moire
	 */
	int _DEBUG::getNumMessages()
	{
		return _DEBUG::a_messages.size();
	}

	/**
	 * Verouille le mutex
	 */
	void _DEBUG::lock()
	{
		_DEBUG::a_mutex.lock();
	}

	/**
	 * Deverouille le mutex
	 */
	void _DEBUG::unlock()
	{
		_DEBUG::a_mutex.unlock();
	}

#endif
