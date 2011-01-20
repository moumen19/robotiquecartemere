#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <iostream>

#include "Camera.hpp"
#include "GenericImageProcessing.hpp"

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
        void Setup();
        void Run();

        // different video processing tools
        void RawDisplay();
        void CannyEdgeDetection();
        void test();

    protected:
        Camera * m_LeftCamera;
        Camera * m_RightCamera;
        vector<cv::Mat > m_LeftImageBuffer;
        vector<cv::Mat > m_RightImageBuffer;
};

#endif // STEREOVISION_H
