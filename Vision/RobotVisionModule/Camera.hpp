#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/highgui/highgui.hpp>

using namespace std;

class Camera: public cv::VideoCapture
{
    public:
        Camera(int cameraIndex = 0);
        Camera(const string& filename);
        virtual ~Camera();

        void loadIntrinsec(const string &filename){;}
        void loadExtrasdec(const string &filename){;}
    protected:
    private:
        cv::Mat intrinsecMatrix;
        cv::Mat extrasinceMatrix;
};

#endif // CAMERA_H
