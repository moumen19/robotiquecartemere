#ifndef CAMSHIFTOBJECTTRACKING_H
#define CAMSHIFTOBJECTTRACKING_H

#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;


void onMouse( int event, int x, int y, int, void* );



class CamShiftObjectTracking
{
    public:
        CamShiftObjectTracking();
        virtual ~CamShiftObjectTracking();

        void help();
        void onMouse( int event, int x, int y, int, void* );
        void Setup();
        void GetObjectCenter(int & center);

    // attributes
    // Required by onMouse event handler
        static Point origin;
        static Rect selection;
        static bool selectObject;
        static int trackObject;
        static Mat image;

    protected:
    private:
        bool backprojMode;
        bool showHist;


        int vmin , vmax, smin;

        /// main
        bool badFrameDetected ;
        VideoCapture cap;
        Rect trackWindow;
        Rect previousGoodWindow;
        RotatedRect trackBox;
        int hsize;
        float hranges[2];// []= {0,180};
        const float* phranges;
        //
        Mat hsv, hue, mask, hist, histimg, backproj;
};

#endif // CAMSHIFTOBJECTTRACKING_H
