
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
//#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
//#include <SFML/System.hpp>
//#include <iostream>

#include "display.h"

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////

int main()
{

    display a;
    a.setSensorDistance(0x90,100);
    sleep(2);

    for(int i=0x30; i<0x50; i=i+2){

         a.setSensorDistance(i,50);
         sleep(2);
    }

//    // Create main window
//    sf::RenderWindow App(sf::VideoMode(800, 600), "SFML Shapes");
//
//    // Start game loop
//    while (App.IsOpened())
//    {
//        // Process events
//        sf::Event Event;
//        while (App.GetEvent(Event))
//        {
//            // Close window : exit
//            if (Event.Type == sf::Event::Closed)
//                App.Close();
//        }
//
//        // Clear screen
//        App.Clear();
//
//        // Draw predefined shapes
//        App.Draw(sf::Shape::Line(390, 590, 710, 10, 2, sf::Color::Red)); // start, end
//
//        // Finally, display the rendered frame on screen
//        App.Display();
//    }

    return EXIT_SUCCESS;
}
