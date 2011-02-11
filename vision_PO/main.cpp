/*
 *
 * Bureau d'�tude Robotique M2 ISEN 2010-2011
 *
 * DELBERGUE Julien
 * JACQUEL Olivier
 * PIETTE Ferdinand (ferdinand.piette@gmail.com)
 *
 * Fichier main.cpp
 *
 */

#include <iostream>
#include <signal.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

#include "Debug.hpp"
#include "./robot/Robot.hpp"

using namespace std;

int main()
{
	_DEBUG_EXEC(configuration(CONSOLE_DISPLAY));	// Configuration du mode Debug

	_DEBUG("Lancement du programme", INFORMATION);

	signal(SIGIO, SIG_IGN); 		// IMPORTANT !! Sinon, le programme plante � la r�ception des donn�es des ports COMs

	Robot robot2010(ACTIVE_MAIN_THREADING);			// Cr�ation du robot
	robot2010.startCommunication();		// Lancement du dialogue avec les capteurs et l'asservissement
	robot2010.start();			// Lancement du traitement des informations et de la planification de trajectoire


	robot2010.wait();			// Mise en attente du robot

	return 0;
}
