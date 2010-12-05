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
	boost::mutex _DEBUG::a_mutex;

	/**
	 * Ajoute un message en mémoire.
	 * Affiche le message dans la console et/ou dans un fichier de logs selon la configuration
	 * @param title - Le titre du message
	 * @param content - Le contenu du message
	 * @param priority - Le niveau de priorite 
	 * @see Debug_Priority
	 * @see _DEBUG()
	 */
	void _DEBUG::addMessage(std::string title, std::string content, Debug_Priority priority)
	{
		// Création du message
		Message m;
		m.title = title;
		m.content = content;
		m.priority = priority;
		m.isEmpty = false;

		// Stockage du message en mémoire
		if((_DEBUG::a_configuration & SAVE_IN_MEMORY) == SAVE_IN_MEMORY)
			_DEBUG::a_messages.push_back(m);	

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
			msg += "\t";
			msg += m.title;
			msg += " :\t";
			msg += m.content;

			// Affichage en console
			if((_DEBUG::a_configuration & CONSOLE_DISPLAY) == CONSOLE_DISPLAY)
				std::cout << msg << std::endl;

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
					std::cerr << "Erreur: Impossible d'ouvrir le fichier de logs du mode DEBUG !" << std::endl;
			}
		}
	}

	/**
	 * Efface tous les messages de la mémoire
 	 */	
	void _DEBUG::clearAllMessage()
	{
		_DEBUG::a_cursor = 0;
		_DEBUG::a_messages.clear();
	}

	/**
 	 * Configure le mode de Deboggage
	 * @param configuration - Les constantes de configuration. Possibilité d'envoyer plusieurs constantes grace a l'operateur OU
	 * @param filename - Le chemin du fichier de logs
	 * @see Debug_Configuration
	 */
	void _DEBUG::configuration(Debug_Configuration configuration, std::string filename = "")
	{
		_DEBUG::a_configuration = configuration;
		_DEBUG::a_filename = filename;
	}

	/**
	 * Retourne le message numero index
	 * @param index - le numero du message stocke en memoire
	 * @return La structure message
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
	 * @return La structure message
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
	 * Retourne le nombre de messages stockés en mémoire
	 * @return Le nombre de message en memoire
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

