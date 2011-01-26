#ifndef STEREOVISION_H
#define STEREOVISION_H


#include <string>
#include <iostream>
#include <fstream>


#include "Camera.hpp"
#include "GenericImageProcessing.hpp"

// for previous functions
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>


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
        void StereoCalibrate();
        void SaveMatrix();
        void LoadMatrix(const string &filename, const string &filename2);

        // Mandatory
        void Setup(int mode);
        void Run();

        // recurrent functions
        bool AcquireFrames();
        void RawDisplay();
        bool GetUserInputs(char key);

        // different video processing tools
        void ProximityMap();
        void CannyEdgeDetection();
        void FloodFilling();
        void test();
        void BlobTracking(cv::Scalar colorToTrack);

        // Features
        void MatchCorners();
        void crossCheckMatching( cv::Ptr<cv::DescriptorMatcher>& descriptorMatcher,
                         const cv::Mat& descriptors1, const cv::Mat& descriptors2,
                         vector<cv::DMatch>& filteredMatches12, int knn=1 );

    protected:
        // core vision attributes
        Camera * m_LeftCamera;
        Camera * m_RightCamera;
        cv::Mat m_LeftFrame;
        cv::Mat m_RightFrame;

        cv::Mat m_rotationMatrix;
        cv::Mat m_translationMatrix;
        cv::Mat m_essentialMatrix;
        cv::Mat m_fundamentalMatrix;

        cv::Mat m_intrinsecMatrix1;
        cv::Mat m_distortionMatrix1;
        cv::Mat m_object_points;
        cv::Mat m_image_points1;
        cv::Size m_board_sz;


        cv::Mat m_intrinsecMatrix2;
        cv::Mat m_distortionMatrix2;
        cv::Mat m_image_points2;


        // optional features
        vector<cv::Mat > m_LeftImageBuffer;
        vector<cv::Mat > m_RightImageBuffer;

        // recording attributes
        cv::VideoWriter m_LeftVideoWriter;
        cv::VideoWriter m_RightVideoWriter;
        bool m_videoBeingRecorded;
        int m_videoOutputCount;
        int m_imageOutputCount;

};

#endif // STEREOVISION_H
