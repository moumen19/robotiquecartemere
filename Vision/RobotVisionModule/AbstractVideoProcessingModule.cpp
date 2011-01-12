#include "AbstractVideoProcessingModule.hpp"

//dtor
AbstractVideoProcessingModule::~AbstractVideoProcessingModule()
{

}

void AbstractVideoProcessingModule::RawVideoDisplay(cv::VideoCapture &cameraToDisplay)
{
    cv::Mat frame;
    cv::namedWindow( "rawDisplay", CV_WINDOW_AUTOSIZE);
    char c = 0;

    while(c<1)
    {
        try{
            cameraToDisplay >> frame;
        }
        catch(std::exception e){
            std::cerr<<e.what(); exit(1);
        }

        if( frame.empty())
         break;

        cv::imshow( "rawDisplay", frame );
        c = (char)cv::waitKey(2);
    }
}
