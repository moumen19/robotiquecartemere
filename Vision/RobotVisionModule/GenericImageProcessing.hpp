#ifndef GENERICIMAGEPROCESSING_H
#define GENERICIMAGEPROCESSING_H

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "Camera.hpp"

using namespace std;

class GenericImageProcessing
{
    public:
        GenericImageProcessing();
        virtual ~GenericImageProcessing();


        static void test(){;}

          // switch (config) {} for many methods ?


        /////////// Edges Detection
        // Sobel
        // Laplace
        static cv::Mat Canny(cv::Mat image, bool settingsActivated);
        static cv::Mat FindContours(){return cv::Mat();} // Only on binary images !
        static cv::Mat ColorSegmentation(cv::Mat imageToSegment, cv::Scalar colorToFind, bool displaySettingsActivated);
        static cv::Mat MorphologyEx(cv::Mat binaryImage, int operation, const cv::Mat& element =cv::Mat(), cv::Point anchor =cv::Point(-1,-1), int iterations =1);

        /////////// Segmentation
        // color segmentation => click on a pixel to set the color center (thresholds 2x HSV)
        // TPs ...

        /////////// Features detection/description/match
        //
        //

        ///////////////////////////////////////////////////
        // getImageFullDescription (channels, depth etc...)


        // HIGH  GUI for tests ! buttons, etc

    protected:
    private:
};

#endif // GENERICIMAGEPROCESSING_H
