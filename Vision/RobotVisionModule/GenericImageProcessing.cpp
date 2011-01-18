#include "GenericImageProcessing.hpp"

 //ctor
GenericImageProcessing::GenericImageProcessing()
{

}

//dtor
GenericImageProcessing::~GenericImageProcessing()
{

}

cv::Mat GenericImageProcessing::Canny(cv::Mat image)
{
    cv::Mat frame;
    cv::cvtColor(image, frame, CV_BGR2GRAY);
    // sigma = 1.5
    // ksize = (sigma * 5)|1 = 7
    cv::GaussianBlur(frame, frame, cv::Size(7,7), 1.5, 1.5);
    cv::Canny(frame, frame, 0, 30, 3);

    return frame;
}
