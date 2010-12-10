/*
 *
 * Bureau d'étude Robotique M2 ISEN 2010-2011
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

#include "Debug.hpp"
#include "./robot/Robot.hpp"

using namespace std;

int main()
{
	_DEBUG_EXEC(configuration(CONSOLE_DISPLAY, "logs.txt"));	// Configuration du mode Debug

	_DEBUG("Lancement du programme", INFORMATION);

	signal(SIGIO, SIG_IGN); 		// IMPORTANT !! Sinon, le programme plante à la réception des données des ports COMs

	Robot robot2010;			// Création du robot
	robot2010.startCommunication();		// Lancement du dialogue avec les capteurs et l'asservissement
	robot2010.start();			// Lancement du traitement des informations et de la planification de trajectoire

	
	//*
	sleep(1);
		int a;
	do{
		std::cout << "Envoyer la commande ? ";
		std::cin >> a;
		std::cout << "Commande envoye..." << std::endl;
		robot2010.test(a);
	}while(a != -1);

	robot2010.stop();//*/

	robot2010.wait();			// Mise en attente du robot

	return 0;
}

