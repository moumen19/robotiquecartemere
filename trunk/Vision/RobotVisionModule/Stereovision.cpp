#include "Stereovision.hpp"

//ctor
Stereovision::Stereovision()
{

}

//dtor
Stereovision::~Stereovision()
{

}

bool Stereovision::setup()
{
    // connect sources
    // connect external...
    return true;
}

void Stereovision::send()
{

}

void Stereovision::display()
{

}

// main routine of the video processing module
void Stereovision::run()
{

   // try{ // the grab of a frame can fail as it is not secured inside the video source classes
    //    getFrame
    cv::VideoCapture cap(0);

    cv::namedWindow( "rawDisplay", 1);
    cv::Mat frame;
    char c = 0;

    while(c<1)
    {
        cap >> frame;
        if( frame.empty())
            break;

        cv::imshow( "rawDisplay", frame );

        c = (char)cv::waitKey(10);
    }
}
