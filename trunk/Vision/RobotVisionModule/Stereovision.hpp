#ifndef STEREOVISION_H
#define STEREOVISION_H

#include "AbstractVideoProcessingModule.hpp"
#include "Camera.hpp"

class Stereovision : public AbstractVideoProcessingModule
{
    public:
        Stereovision();
        virtual ~Stereovision();

        virtual bool setup();
        virtual void run();
        virtual void send();
        virtual cv::Mat getFrame();
        virtual void display();
    protected:
    private:
        Camera * m_LeftCamera;
        Camera * m_RightCamera;
};

#endif // STEREOVISION_H
