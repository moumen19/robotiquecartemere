
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
    //system1.FloodFilling();
    //system1.CannyEdgeDetection();
    //system1.MatchCorners();
    //system1.ProximityMap();
    //system1.test();

    return 0;
}

