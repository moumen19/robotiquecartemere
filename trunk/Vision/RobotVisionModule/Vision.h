#ifndef VISION_H
#define VISION_H

#include <Camera.h>

class Vision
{
    public:
        Vision();
        virtual ~Vision();
    protected:
    private:
        Camera * m_left;
        Camera * m_right;
};

#endif // VISION_H
