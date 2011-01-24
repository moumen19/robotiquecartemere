#ifndef CAMERA_H
#define CAMERA_H

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <iostream>
#include <fstream>


///////////////// Values for recording modes ////////////
#define WAITING_TIME_MS 2 // 2 ms minimum between frames
#define RECORDING_FPS 15.0

#define STOP_DISPLAY_KEY 27 // Escape
#define VIDEO_RECORD_KEY 118 // V
#define IMAGE_RECORD_KEY 105 // I
////////////////////////////////////////////////////////

using namespace std;
using namespace cv;



class Camera: public cv::VideoCapture
{
    public:
        Camera();
        Camera(int cameraIndex);
        Camera(const string& filename);
        virtual ~Camera();

        // not presently used
        void LiveDisplay();
        void SetImageCapture(bool activated);
        void SetVideoCapture(bool activated);

        // useful for stereovision
        void CalibrateFromImageSet(){;}
        void CalibrateFromCamera();
        void UndistorFrame(){;} // useful, even if imperfections can't be seen?
        void LoadIntrinsec(const string &filename){;}
        void LoadExtrasec(const string &filename){;}
        void SaveMatrix(const string &filename);


    protected:
        string m_Name;
        // communication protocole: e.g USB for camera, none for file
        cv::Mat m_intrinsecMatrix;
        cv::Mat m_distortionMatrix;

        // to record frames
        bool m_ImageCaptureActivated;
        bool m_VideoCaptureActivated;
};

#endif // CAMERA_H
