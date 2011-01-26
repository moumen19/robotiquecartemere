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

#define nbImages 19
#define nbLines 6
#define nbColumns 8
#define nbCorners nbLines*nbColumns

#define ratioPixelOverUnit 3 // size of a single square = 3x3 (cm)
#define D 60 // ball real diameter (mm)


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
        void CalibrateFromImageSet();
        void compute_and_display_image_corners(char * imageName, CvSize * imageSize, CvSize chessboardSize, CvPoint2D32f * cornersArrayToFillIn);
        void initiate_matrices_for_calibration(CvMat * intrinsicMatrix, CvMat * distortionCoeffs,CvMat * nbTotalCorners, CvMat * objectRealCoordinates,CvMat * cornersMat, CvPoint2D32f * cornersList);
        void improve_precision(IplImage *image, CvPoint2D32f * cornersArray, int cornersCount);

        void CalibrateFromCamera();
        void UndistorFrame(){;} // useful, even if imperfections can't be seen?
        void LoadIntrinsec(const string &filename){;}
        void LoadExtrasec(const string &filename){;}
        void SaveMatrix(const string &filename);
        void test();


    protected:
        string m_Name;
        // communication protocole: e.g USB for camera, none for file
        cv::Mat m_intrinsecMatrix;
        cv::Mat m_distortionMatrix;
        cv::Mat m_object_points;
        cv::Mat m_image_points;
        cv::Size m_board_sz;

        cv::Mat m_intrinsecMatrix_Right;
        cv::Mat m_intrinsecMatrix_Left;
        cv::Mat m_distortionMatrix_Right;
        cv::Mat m_distortionMatrix_Left;

        // to record frames
        bool m_ImageCaptureActivated;
        bool m_VideoCaptureActivated;
};

#endif // CAMERA_H
