#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <iostream>

#include "AbstractVideoProcessingModule.hpp"

class Stereovision : public AbstractVideoProcessingModule
{
    public:
        Stereovision();
        virtual ~Stereovision();

        virtual bool setup();
        virtual void run();
        virtual void send();
        virtual void display();
    protected:
    private:
};

#endif // STEREOVISION_H
