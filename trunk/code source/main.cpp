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

#include "Debug.hpp"
#include "./robot/Robot.hpp"

using namespace std;

int main()
{
	// Configuration du mode Debug
	_DEBUG_EXEC(configuration(CONSOLE_DISPLAY, "logs.txt"));


	_DEBUG("Lancement du programme", INFORMATION);

	signal(SIGIO, SIG_IGN); // IMPORTANT !! Sinon, le programme plante � la r�ception des donn�es des ports COMs

	// Cr�ation du robot
	Robot robot2010;
	// Lancement du calcul du robot
	robot2010.startCommunication();
	robot2010.start();

	
	/*sleep(1);
		int a;
	do{
		std::cout << "Envoyer la commande ? ";
		std::cin >> a;
		std::cout << "Commande envoye..." << std::endl;
		robot2010.test();
	}while(a != 0);

	robot2010.stop();*/
	robot2010.wait();

	return 0;
}

