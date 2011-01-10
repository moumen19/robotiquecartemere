#ifndef CAMERA_H
#define CAMERA_H

#include "AbstractVideoSource.hpp"


class Camera : public AbstractVideoSource
{
    public:
        // ctor takes the index of the device as a parameter
        Camera(int index = -1);

        virtual ~Camera();

        // required: inheritance from abstract
        virtual void open();
        virtual void close();
        virtual cv::Mat getFrame();

    protected:
    private:
        // index of camera
        int m_index;

        // a video source in opencv
        CvCapture * m_Capture;

};

#endif // CAMERA_H
