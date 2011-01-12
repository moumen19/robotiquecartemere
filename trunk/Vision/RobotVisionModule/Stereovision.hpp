#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <iostream>
#include "AbstractVideoProcessingModule.hpp"

using namespace std;

class Stereovision : public AbstractVideoProcessingModule
{
    public:
        Stereovision();
        virtual ~Stereovision();

        virtual void Setup();
        virtual void Run();
        virtual void Send(){;}
    protected:
    private:
        cv::VideoCapture * m_LeftCamera;
        cv::VideoCapture * m_RightCamera;
};

#endif // STEREOVISION_H
