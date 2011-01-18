#ifndef GENERICIMAGEPROCESSING_H
#define GENERICIMAGEPROCESSING_H

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Camera.hpp"

using namespace std;

class GenericImageProcessing
{
    public:
        GenericImageProcessing();
        virtual ~GenericImageProcessing();


        static void test(){;}

          // switch (config) {} for many methods ?


        /////////// edge detection
        // Sobel
        // Laplace
        static cv::Mat Canny(cv::Mat image);
        // return findContours

        /////////// features detection/description/match
        //
        //

        ////////////////////////////////
        // getImageFullDescription (channels, depth etc...)

        // TPs !!

//        static qqch(cv::Mat)
//        static qqch(CameraS)

        // HIGH  GUI for tests !

    protected:
    private:
};

#endif // GENERICIMAGEPROCESSING_H
