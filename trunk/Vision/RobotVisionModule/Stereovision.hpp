#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <iostream>

#include "Camera.hpp"

using namespace std;

class Stereovision
{
    public:
        Stereovision();
        virtual ~Stereovision();

        void Calibrate () {;}
        virtual void Send(){;}
        virtual void Setup();
        virtual void Run();
        void RawVideoDisplay();
    protected:
    private:
        Camera * m_LeftCamera;
        Camera * m_RightCamera;

};

#endif // STEREOVISION_H
