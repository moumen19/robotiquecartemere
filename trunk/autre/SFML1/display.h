#ifndef DISPLAY_H
#define DISPLAY_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#define windowSizeW 800
#define windowSizeH 600

#define Id_BAU 0x90
#define Id_US_min 0x30
#define Id_US_max 0x4F
#define Id_IR_min 0x50
#define Id_IR_max 0x6F
#define Id_Vide_min 0x70
#define Id_Vide_max 0x8F


class display
{
    public:
        display();
        ~display();

        void Close();
        void setSensorDistance(int idSensor, float dist);

        void changeDistanceBAU(int dist);  // pour le BAU, toute valeur différente de 0 est considérée comme positive ! (sécurité)
        void changeDistanceUS(int id, float dist);
        void changeDistanceIR(int id, float dist);
        void changeDistanceVide(int id, float dist);

    private:

         void initializeDistanceValues();

         std::map<int, float> _currentDistance;
         sf::RenderWindow * _window;

};

#endif // DISPLAY_H
