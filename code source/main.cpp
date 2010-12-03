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
    // Configuration du mode Debug
    _DEBUG_EXEC(configuration(DISPLAY_NONE, "logs.txt"));


    _DEBUG("Lancement du programme", INFORMATION);

    signal(SIGIO, SIG_IGN); // IMPORTANT !! Sinon, le programme plante à la réception des données des ports COMs

    // Création du robot
    Robot robot2010;
    // Lancement du calcul du robot
    robot2010.startCommunication();
    robot2010.start();
    robot2010.wait();

    return 0;
}

