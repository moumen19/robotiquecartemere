#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <iostream>

#include "AbstractVideoProcessingModule.hpp"
#include "Camera.hpp"

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
        Camera * m_LeftCamera;
        Camera * m_RightCamera;
};

#endif // STEREOVISION_H
