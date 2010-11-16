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

#include "Debug.hpp"
#include "./robot/Robot.hpp"

using namespace std;

int main()
{
    // Configuration du mode Debug
    _DEBUG_EXEC(configuration(CONSOLE_DISPLAY | FILE_DISPLAY, "logs.txt"));


    _DEBUG("Main.cpp", "Lancement du programme", INFORMATION);

    // Création du robot
    Robot robot2010;
    // Lancement du calcul du robot
    robot2010.start();
    robot2010.wait();

    return 0;
}
