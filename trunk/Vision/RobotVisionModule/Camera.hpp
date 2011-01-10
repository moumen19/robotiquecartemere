#ifndef CAMERA_H
#define CAMERA_H

#include "AbstractVideoSource.hpp"


class Camera : public AbstractVideoSource
{
    public:
        Camera();
        virtual ~Camera();
    protected:
    private:
};

#endif // CAMERA_H
