#include "Stereovision.hpp"

//ctor
Stereovision::Stereovision():m_LeftCamera(NULL),m_RightCamera(NULL)
{
}

//dtor
Stereovision::~Stereovision()
{
    delete m_LeftCamera;
    delete m_RightCamera;
}

// connect sources, external..
void Stereovision::Setup()
{
    try{
        // setup left camera
        m_LeftCamera = new Camera("webcam1.avi");
        if(!m_LeftCamera->isOpened())
           throw (new std::exception());

        // setup right camera
        m_RightCamera = new Camera("webcam2.avi");
        if(!m_RightCamera->isOpened())
            throw (new std::exception());

    }
    catch(std::exception e){
        cerr<<e.what(); exit(1);
    }
}


// display both cameras untouched
void Stereovision::RawVideoDisplay()
{
    cv::Mat frame;
    cv::namedWindow( "rawDisplay_Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "rawDisplay_Right", CV_WINDOW_AUTOSIZE);
    char c = 0;

    while(c<1)
    {
        c = (char)cv::waitKey(2); // shortest possible delay(experience result)

        // left
        *m_LeftCamera >> frame;

        if(frame.empty()) break;

        cv::imshow( "rawDisplay_Left", frame );

        // right
        *m_RightCamera >> frame;

        if(frame.empty())break;

        cv::imshow( "rawDisplay_Right", frame );
    }
}

// main routine of the video processing module
void Stereovision::Run()
{
    RawVideoDisplay();

}
