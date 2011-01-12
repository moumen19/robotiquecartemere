#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui/highgui.hpp>

class Camera: public cv::VideoCapture
{
    public:
        Camera(unsigned int index = 0);
        virtual ~Camera();
    protected:
    private:
        cv::Mat intrinsecMatrix;
        cv::Mat extrasinceMatrix;
};

#endif // CAMERA_H
