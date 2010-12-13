#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

#include "display.h"

display::display()
{
    _window = new sf::RenderWindow();
    _window->Create(sf::VideoMode(windowSizeW, windowSizeH), "SFML en tombant :)");
    _window->Clear();

    _currentDistance.clear();
    initializeDistanceValues();
}

// Dépend
void display::initializeDistanceValues()
{
    // BAU
    _currentDistance[0x90] = 0;

    // US
    _currentDistance[0x30] = 100;
    _currentDistance[0x32] = 100;
    _currentDistance[0x34] = 100;
    _currentDistance[0x36] = 100;
    _currentDistance[0x38] = 100;
    _currentDistance[0x3A] = 100;
    _currentDistance[0x3C] = 100;
    _currentDistance[0x3E] = 100;

    // IR
    _currentDistance[0x50] = 100;
    _currentDistance[0x53] = 100;
    _currentDistance[0x56] = 100;
    _currentDistance[0x59] = 100;

    // Vide
    _currentDistance[0x70] = 100;
    _currentDistance[0x72] = 100;

    // Ecrit les Id capteurs
    for (std::multimap<int, float>::iterator it = _currentDistance.begin(); it != _currentDistance.end(); ++it)
    {
	       std::cout << "  [" << (*it).first << ", " << (*it).second << "]" << std::endl;
    }
}

display::~display()
{
    delete _window;
}

void display::Close()
{
    _window->Close();
}


void display::setSensorDistance(int idSensor, float dist) // if-else plutot que switch pour définir des plages de valeurs plutot que des ID individuelles (modulable facilement)
{

    if(idSensor == Id_BAU){ // BAU
        changeDistanceBAU(dist);
    }
    else if(idSensor >= 0x30 && idSensor <= 0x4F){ // US
        changeDistanceUS(idSensor, dist);
    }
    else if(idSensor >= 0x50 && idSensor <= 0x6F){ // IR
        changeDistanceIR(idSensor, dist);
    }
    else if(idSensor >= 0x70 && idSensor < 0x8F){ // Vide
        changeDistanceVide(idSensor, dist);
    }
    else{
        std::cout<<"PB SENSOR ID !"<<std::endl;
    }
    _window->Display();
}

 void display::changeDistanceBAU(int dist)
 {
    if(dist ==0 )
        _window->Draw(sf::Shape::Rectangle(10, 10, 100, 100, sf::Color::Black ));
    else
        _window->Draw(sf::Shape::Rectangle(10, 10, 100, 100, sf::Color::Red ));
 }

void display::changeDistanceUS(int id, float dist)
{
    if(_currentDistance.find(id) == _currentDistance.end()){
        std::cout<<"PB ID US !"<<std::endl;
        exit(0);
    }

     if(_window->IsOpened()){

        sf::Shape lineToDraw = sf::Shape::Line( (id-Id_US_min +1)*30 , // x1
                                                 windowSizeH -50, // y1
                                                  (id-Id_US_min +1)*30, // x2
                                                 windowSizeH- _currentDistance[id] -50, // y2
                                                 3,
                                                 sf::Color::Red);
         _currentDistance[id]=dist; // Update de la valeur

         // efface la position précédente
        _window->Draw(lineToDraw);

       /* lineToDraw.SetPointPosition(1, // 2ème point
                                    lineToDraw.GetPointPosition(1).x, // X inchangé
                                    windowSizeH- _currentDistance[id]); // nouvel Y
        lineToDraw.SetColor(sf::Color::Blue);

        // dessine la nouvelle position
        _window->Draw(lineToDraw);*/
      }
 }

void display::changeDistanceIR(int id, float dist)
{
    sf::Shape lineToDraw = sf::Shape::Line(windowSizeW/2 ,windowSizeH -1, windowSizeW/2 , windowSizeH-dist, 3, sf::Color::Green);
}

void display::changeDistanceVide(int id, float dist)
{
    sf::Shape lineToDraw = sf::Shape::Line(windowSizeW/2 ,windowSizeH -1, windowSizeW/2 , windowSizeH-dist, 3, sf::Color::Yellow);
}
