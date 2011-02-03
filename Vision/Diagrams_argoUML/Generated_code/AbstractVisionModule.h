#ifndef ABSTRACTVISIONMODULE_H
#define ABSTRACTVISIONMODULE_H

// FILE: D:/TRAVAIL/M2/Robotique_Avancee/SVN_partie_info/Vision/Diagrams_argoUML/Generated_code//AbstractVisionModule.h

#include "VideoSource.h"


class AbstractVisionModule {
    /* {src_lang=cpp}*/


    // Operations
 public:

    virtual void acquireFrame();

    virtual bool Setup();

    virtual void Run();

    // Attributes
 public:
    VideoSource sources;

    // Associations
 public:

    /**
     * @element-type VideoSource
     */
    VideoSource *myVideoSource;

//end of class AbstractVisionModule
};

#endif // ABSTRACTVISIONMODULE_H
