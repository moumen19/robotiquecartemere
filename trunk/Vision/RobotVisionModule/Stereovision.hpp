#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <iostream>

#include "Camera.hpp"
#include "GenericImageProcessing.hpp"

#define CAMERA_MODE 0
#define VIDEO_FILE_MODE 1

using namespace std;

class Stereovision
{
    public:
        Stereovision();
        virtual ~Stereovision();

        // To implement
        virtual void Send(){;}
        void CalibrateStereo () {;}

        // Mandatory
        void Setup(int mode);
        void Run();

        // different video processing tools
        void RawDisplay();
        void CannyEdgeDetection();
        void FloodFilling();
        void test();

    protected:
        Camera * m_LeftCamera;
        Camera * m_RightCamera;
        vector<cv::Mat > m_LeftImageBuffer;
        vector<cv::Mat > m_RightImageBuffer;
};

#endif // STEREOVISION_H
