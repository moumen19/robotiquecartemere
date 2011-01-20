
#include <stdio.h>
#include <iostream>

#include "Stereovision.hpp"

int main( int argc, char **argv )
{
    Stereovision system1;
    system1.Setup();
    system1.CannyEdgeDetection();
    //system1.RawDisplay();

//    Camera cam1;
//    cam1.SetImageCapture(true);
//    cam1.SetVideoCapture(true);
//    cam1.LiveDisplay();


    return 0;
}


