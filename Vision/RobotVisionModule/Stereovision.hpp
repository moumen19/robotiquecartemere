#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <string>
#include <iostream>
#include <fstream>

#include "Camera.hpp"


#include <opencv/cv.h> // for previous functions
#include <opencv2/highgui/highgui.hpp>

// much of the define are in Camera.hpp
#define CAMERA_MODE 0
#define VIDEO_FILE_MODE 1


using namespace std;

class Stereovision
{
    public:
        Stereovision();
        virtual ~Stereovision();

        // To implement !
        virtual void Send(){;} // through Ethernet for instance
        void StereoCalibrate();
        bool SaveMatrix();
        bool LoadStereoMatrices(const string &filename);

        // Mandatory
        void Setup(int mode);
        void Run(); // empty for now, replaced by:

        // different video processing tools
        void RawDisplay();
        void CannyEdgeDetection();
        void FloodFilling(); // Seed has to be a parameter
        void BlobTracking(cv::Scalar colorToTrack);
        void test(); // not working


    private:
        // recurrent functions
        bool AcquireFrames();
        bool GetUserInputs(char key); // features recording options, videos or images

        // different image processing tools with optional display
        cv::Mat Canny(cv::Mat image, bool settingsActivated);
        cv::Mat MorphologyEx(cv::Mat binaryImage, int operation, // mandatory
                             const cv::Mat& element =cv::Mat(), cv::Point anchor =cv::Point(-1,-1), int iterations =1);
        cv::Mat ColorSegmentation(cv::Mat imageToSegment, cv::Scalar colorToFind, bool displaySettingsActivated);// better results with MorphologyEx
        cv::Mat FindContours(bool displaySettingsActivated){return cv::Mat();} // can be useful to implement, but works only on binary images !

        // for corners detection/matching purpose (unused)
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

        // optional dynamic buffer
        vector<cv::Mat > m_LeftImageBuffer;
        vector<cv::Mat > m_RightImageBuffer;

        // stereo attributes
        cv::Mat m_rotationMatrix;
        cv::Mat m_translationMatrix;
        cv::Mat m_essentialMatrix;
        cv::Mat m_fundamentalMatrix;

        // recording attributes
        cv::VideoWriter m_LeftVideoWriter;
        cv::VideoWriter m_RightVideoWriter;
        bool m_videoBeingRecorded;
        int m_videoOutputCount;
        int m_imageOutputCount;

};

#endif // STEREOVISION_H
