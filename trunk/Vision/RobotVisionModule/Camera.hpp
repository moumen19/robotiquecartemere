#ifndef CAMERA_H
#define CAMERA_H

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <iostream>
#include <fstream>


#define WAITING_TIME_MS 2 // 2 ms minimum between frames

// Values for recording modes ////////
#define RECORDING_FPS 15.0
#define STOP_DISPLAY_KEY 27 // Escape
#define VIDEO_RECORD_KEY 118 // V
#define IMAGE_RECORD_KEY 105 // I
//////////////////////////////////////


// to remove //////////////////////////////////////////////////////
#define nbImages 19
#define nbLines 6
#define nbColumns 8
#define nbCorners nbLines*nbColumns

#define offsetX 1
#define offsetY 1

#define ratioPixelOverUnit 3 // size of a single square = 3x3 (cm)
#define D 60 // ball real diameter (mm)
///////////////////////////////////////////////////////////////////


using namespace std;
using namespace cv;


class Camera: public cv::VideoCapture
{
    public:

        Camera();
        Camera(int cameraIndex);
        Camera(const string& filename);
        virtual ~Camera();

        // basic display of the video stream, which can be recorded ('i'mage and/or 'v'ideo)
        void LiveDisplay();

        // frome C-langage. Need to be changed ?
        void CalibrateFromCamera();
        void CalibrateFromImageSet();

        // to be implemented
        void Undistort(){;} // useful, even if imperfections can't be seen?
        void SaveMatrix(const string &filename);
        void LoadMatrices(const string &filename){;}


    protected:
        // communication protocole to add here ?
        // e.g USB for camera, none for file?

        string m_Name; // formed with the constructor

        // attributes for camera device calibration
        cv::Mat m_intrinsecMatrix;
        cv::Mat m_distortionMatrix;

        // those are to remove ?
        cv::Mat m_chessboardplanCoordinates;
        cv::Mat m_cornersMat;
        cv::Mat m_nbTotalCorners;
        cv::Size m_image_size;
        CvSize image_size;
};

#endif // CAMERA_H
