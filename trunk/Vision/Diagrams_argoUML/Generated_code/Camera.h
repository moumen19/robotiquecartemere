#ifndef CAMERA_H
#define CAMERA_H

// FILE: D:/TRAVAIL/M2/Robotique_Avancee/SVN_partie_info/Vision/Diagrams_argoUML/Generated_code//Camera.h

#include "VideoSource.h"


class Camera : public VideoSource {
    /* {src_lang=cpp}*/


    // Operations
 public:

    virtual void Calibrate();

    virtual void SaveMatrices();

    virtual void LoadMatrices();

    // Attributes

 private:
    int newAttr;

//end of class Camera
};

#endif // CAMERA_H
