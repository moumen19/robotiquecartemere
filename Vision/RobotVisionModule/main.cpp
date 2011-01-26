
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


      Camera camL;
      camL.CalibrateFromImageSet();
//    Camera camR(1);
//    camL.CalibrateFromCamera();
//    camL.SaveMatrix("gauche");
//    camR.CalibrateFromCamera();
//    camR.SaveMatrix("droite");
//    cam1.LiveDisplay();


    // Just for tests: MEMORY LEAKS !!
//    int ballsPixelsSizes =0;
//    get_balls_pixelsSizes( &ballsPixelsSizes, "//host//TRAVAIL//M2//Vision//Echantillons//stereo_images//Img_right_4.jpg");
//
//    freeGlobalVariables();

    return 0;
}


