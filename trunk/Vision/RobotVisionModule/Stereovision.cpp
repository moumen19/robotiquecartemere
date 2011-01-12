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
    m_LeftCamera = new cv::VideoCapture(0);
    if(!m_LeftCamera->isOpened())
        throw (new std::exception());

    // setup right camera
//    m_RightCamera = new cv::VideoCapture(1);
//    if(!m_RightCamera->isOpened())
//        exit(1);
    }
    catch(std::exception e){
        cerr<<e.what(); exit(1);
    }
}

// main routine of the video processing module
void Stereovision::Run()
{
    RawVideoDisplay(*m_LeftCamera);
}

