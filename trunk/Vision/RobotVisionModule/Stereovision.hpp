#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/objdetect/objdetect.hpp>

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
