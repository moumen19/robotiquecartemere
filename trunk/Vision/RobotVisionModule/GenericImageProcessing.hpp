#ifndef GENERICIMAGEPROCESSING_H
#define GENERICIMAGEPROCESSING_H

#include "Camera.hpp"
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
        // Canny
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
