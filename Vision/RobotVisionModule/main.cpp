
#include <iostream>
#include "Stereovision.hpp"



int main( int argc, char **argv )
{
//    Stereovision system1;
//    system1.Setup(CAMERA_MODE);
//    system1.RawDisplay();

    //system1.FloodFilling();
    //system1.CannyEdgeDetection();
    //system1.MatchCorners();
    //system1.ProximityMap();
    //system1.test();

    Camera cam1(0);
    Camera cam2(1);
    cam1.CalibrateFromCamera();
    cam1.SaveMatrix("gauche");
    cam2.CalibrateFromCamera();
    cam2.SaveMatrix("droite");
//    cam1.LiveDisplay();


    // Just for tests: MEMORY LEAKS !!
//    int ballsPixelsSizes =0;
//    get_balls_pixelsSizes( &ballsPixelsSizes, "//host//TRAVAIL//M2//Vision//Echantillons//stereo_images//Img_right_4.jpg");
//
//    freeGlobalVariables();

    return 0;
}


