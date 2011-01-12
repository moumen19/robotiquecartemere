#ifndef ABSTRACTVIDEOPROCESSINGMODULE_H
#define ABSTRACTVIDEOPROCESSINGMODULE_H

#include <vector>

#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>

#include "CircularBuffer.hpp"

using namespace std;

// no ctor and only purely virtual methods: it is an abstract class and can't be instancied
class AbstractVideoProcessingModule
{
    public:
        virtual ~AbstractVideoProcessingModule();

        virtual void Setup() =0; // setup sources
        virtual void Run() =0; // execute the processing routine
        virtual void Send() =0;
        void RawVideoDisplay(cv::VideoCapture &cameraToDisplay); // show the raw video stream
    protected:
        // dynamical buffer of stored images
        vector<cv::Mat * > m_imageBuffer;

        // AbstractCommunicationProtocol * m_protocol;
        // to implement
};

#endif // ABSTRACTVIDEOPROCESSINGMODULE_H
