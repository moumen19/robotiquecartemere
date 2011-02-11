
#include <iostream>
#include "Camera.hpp"
#include "Stereovision.hpp"

int main( int argc, char **argv )
{
    // cr�ation des cam�ra
    Camera Left_cam(0);
    Camera Right_cam(1);

    // chargement des matrices intrins�ques/distortion
    Left_cam.LoadMatrices("left_cam_matrices.xml");
    Right_cam.LoadMatrices("right_cam_matrices.xml");

    // cr�ation du syst�me de st�r�o associ� aux cam�ras physiques
    Stereovision system1;

    // passage des cam�ras par r�f�rence
    system1.SetCameras(Left_cam, Right_cam);

    // chargement des matrices de calibration du syst�me
    // associ� aux cam�ras pr�c�dentes
    system1.LoadStereoMatrices("stereosystem_matrices");

    // ex�cution de la routine principale en continu
    system1.Run();
    //system1.FloodFilling();
    //system1.CannyEdgeDetection();
    //system1.MatchCorners();
    //system1.ProximityMap();
    //system1.test();

    return 0;
}

