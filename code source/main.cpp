#include <iostream>

#include "Debug.hpp"
#include "./robot/Robot.hpp"

using namespace std;

int main()
{
    // Configuration du mode Debug
    if(_DEBUG_MODE)
    {
        _DEBUG::configuration(CONSOLE_DISPLAY | FILE_DISPLAY, "logs.txt");
        _DEBUG::addMessage("Main.cpp", "Lancement du programme", INFORMATION);
    }

    // Création du robot
    Robot robot2010;

    // Lancement du calcul du robot
    while(1)
        robot2010.run();


    return 0;
}
