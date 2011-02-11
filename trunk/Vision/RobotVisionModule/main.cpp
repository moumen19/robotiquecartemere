
#include <iostream>
#include "Camera.hpp"
#include "Stereovision.hpp"

int main( int argc, char **argv )
{
    // création des caméra
    Camera Left_cam(0);
    Camera Right_cam(1);

    // chargement des matrices intrinsèques/distortion
    Left_cam.LoadMatrices("left_cam_matrices.xml");
    Right_cam.LoadMatrices("right_cam_matrices.xml");

    // création du système de stéréo associé aux caméras physiques
    Stereovision system1;

    // passage des caméras par référence
    system1.SetCameras(Left_cam, Right_cam);

    // chargement des matrices de calibration du système
    // associé aux caméras précédentes
    system1.LoadStereoMatrices("stereosystem_matrices");

    // exécution de la routine principale en continu
    system1.Run();

    return 0;
}
    //system1.FloodFilling();
    //system1.CannyEdgeDetection();
    //system1.MatchCorners();
    //system1.ProximityMap();
    //system1.test();



//   Camera camL;
//   camL.CalibrateFromImageSet();
//
//   system1.LoadMatrix("left_camera.xml","right_camera.xml");
//   system1.StereoCalibrate();
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




