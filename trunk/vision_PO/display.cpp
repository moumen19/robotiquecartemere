
#include <iostream>
#include <sstream>

#include "display.h"

display::display()
{
    _window = new sf::RenderWindow();
    _window->Create(sf::VideoMode(windowSizeW, windowSizeH), "SFML en tombant :)");
    _window->Clear();

    initializeDistanceValues();
}

void display::initializeDistanceValues()
{
    _currentDistance.clear();

    // BAU
    _currentDistance[144] = 0;

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
}

display::~display()
{
    Close();
    delete _window;
}

void display::Close()
{
    _window->Close();
}

// if-else plutot que switch pour définir des plages de valeurs plutot que des ID individuelles (modulable facilement)
void display::setSensorDistance(int idSensor, float dist)
{
    // Verification de l'ID capteur dans la liste préalablement définie
    if(_currentDistance.find(idSensor) == _currentDistance.end()){
        std::cout<<"PB SENSOR ID ! "<<idSensor<<std::endl;
        exit(0);
    }

    //*
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
        std::cout<<"PB SENSOR ID (switch) !"<<std::endl;
          exit(0);
    }//*/

    _window->Display();
}

// Numéro du capteur sous la jauge (doit pouvoir etre affiché en hexa avec les options du "ostringstream")
void display::writeId(int Id, int d, float Xposition, sf::Color col)
{
        std::ostringstream stm;
        stm << Id;

        sf::String text;
        text.SetText(stm.str());
        text.SetSize(15);
        text.SetPosition(Xposition-15,windowSizeH-40);
        text.SetColor(col);

        _window->Draw(text);

	std::ostringstream stm2;
	stm2 << d;
	text.SetText(stm2.str());
        text.SetSize(15);
        text.SetPosition(Xposition-25,windowSizeH-20);
        text.SetColor(col);

	_window->Draw(text);
}

// ROUGE: BAU
 void display::changeDistanceBAU(int dist)
 { 
    //_window->Clear();
    if(dist ==0 ){
        _window->Draw(sf::Shape::Rectangle(10, 10, 350, 80, sf::Color::Black ));
    }
    else{
        _window->Draw(sf::Shape::Rectangle(10, 10, 350, 80, sf::Color::Red ));
        sf::String text;
        text.SetText("ARRET URGENCE");
        text.SetSize(35);
        text.SetPosition(15,30);
        text.SetColor(sf::Color::White);

        _window->Draw(text);
    }
 }

// Vert: Ultrasons
void display::changeDistanceUS(int id, float dist)
{
	sf::Color color = sf::Color::Green;
	
	if(dist > 2000)
	{
		color = sf::Color::Red;
		dist = 2000;
	}
	else if(dist <= 150)
	{
		color = sf::Color::Red;
		if(dist != 0)
			dist = 140;
	}
     //std::cout << dist << std::endl;
	//_window->Clear();
        sf::Shape RectToErase = sf::Shape::Rectangle( (id-Id_US_min +1)*30 - 30, // x1
                                                 windowSizeH, // y1
                                                  (id-Id_US_min +1)*30 +30, // x2
                                                 (windowSizeH- (int)_currentDistance[id] -53) * ratioUS, // y2
                                                 sf::Color::Black);
         // efface la position précédente
        _window->Draw(RectToErase);

        // Update de la valeur
        _currentDistance[id]=dist;

        sf::Shape lineToDraw = sf::Shape::Line( (id-Id_US_min +1)*30, // x1
                                                 windowSizeH -50, // y1
                                                  (id-Id_US_min +1)*30, // x2
                                                 windowSizeH- _currentDistance[id]*ratioUS -50, // y2
                                                 3,
                                                 color);
        // dessine la nouvelle position
        _window->Draw(lineToDraw);

        // écrit l'ID => à refaire, ne devrait l'afficher qu'une seule fois car ne change plus
        writeId(id, _currentDistance[id],
                (id-Id_US_min +1)*30, // x
                  color); // y

 }

// Bleu: Infrarouges
void display::changeDistanceIR(int id, float dist)
{
       if(_window->IsOpened()){

        sf::Shape RectToErase = sf::Shape::Rectangle( (id-Id_IR_min +1)*30 - 5 +500 , // x1
                                                 windowSizeH -47, // y1
                                                  (id-Id_IR_min +1)*30 +5 +500, // x2
                                                 (windowSizeH- (int)_currentDistance[id] -53) * ratioIR, // y2
                                                 sf::Color::Black);
         // efface la position précédente
        _window->Draw(RectToErase);

        // Update de la valeur
        _currentDistance[id]=dist;

        sf::Shape lineToDraw = sf::Shape::Line( (id-Id_IR_min +1)*30 +500, // x1
                                                 windowSizeH -50, // y1
                                                  (id-Id_IR_min +1)*30 +500, // x2
                                                 (windowSizeH- _currentDistance[id] -50) * ratioIR, // y2
                                                 3,
                                                 sf::Color::Blue);
        // dessine la nouvelle position
        _window->Draw(lineToDraw);

        // écrit l'ID => à refaire, ne devrait l'afficher qu'une seule fois car ne change plus
        writeId(id,(int)_currentDistance[id],
                (id-Id_IR_min +1)*30 +500, // x
                  sf::Color::Blue); // y
      }
}

// JAUNE: Capteur du vide
void display::changeDistanceVide(int id, float dist)
{
      if(_window->IsOpened()){

        sf::Shape RectToErase = sf::Shape::Rectangle( (id-Id_Vide_min +1)*30 - 5 +900, // x1
                                                 windowSizeH -47, // y1
                                                  (id-Id_Vide_min +1)*30 +5 +900, // x2
                                                 (windowSizeH- (int)_currentDistance[id] -53) * ratioVide, // y2
                                                 sf::Color::Black);
         // efface la position précédente
        _window->Draw(RectToErase);

        // Update de la valeur
        _currentDistance[id]=dist;

        sf::Shape lineToDraw = sf::Shape::Line( (id-Id_Vide_min +1)*30 +900, // x1
                                                 windowSizeH -50, // y1
                                                  (id-Id_Vide_min +1)*30 +900, // x2
                                                 (windowSizeH- _currentDistance[id] -50) * ratioVide, // y2
                                                 3,
                                                 sf::Color::Yellow);
        // dessine la nouvelle position
        _window->Draw(lineToDraw);

        // écrit l'ID => à refaire, ne devrait l'afficher qu'une seule fois car ne change plus
        writeId(id,(int)_currentDistance[id],
                (id-Id_Vide_min +1)*30 +900, // x
                  sf::Color::Yellow); // y
      }
}
