#ifndef ABSTRACTVIDEOPROCESSINGMODULE_H
#define ABSTRACTVIDEOPROCESSINGMODULE_H

#include <vector>

#include <opencv2/video/tracking.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;

// no ctor and only purely virtual methods: it is an abstract class and can't be instancied
class AbstractVideoProcessingModule
{
    public:
        virtual ~AbstractVideoProcessingModule();

        virtual bool setup() =0; //setup sources
        virtual void run() =0;
        virtual void send() =0;
        virtual void display() =0;
    protected:
    private:
        // dynamical array of video sources, in ordrer to be modulable
        vector<cv::VideoCapture * > m_VideoSources;

        // dynamical buffer of stored images
        vector<cv::Mat * > m_imageBuffer;

        // AbstractCommunicationProtocol * m_protocol;
        // to implement
};

#endif // ABSTRACTVIDEOPROCESSINGMODULE_H
