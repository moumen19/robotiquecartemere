#ifndef ABSTRACTVIDEOPROCESSINGMODULE_H
#define ABSTRACTVIDEOPROCESSINGMODULE_H


#include <vector>
#include <opencv/cv.h>

class AbstractVideoProcessingModule
{
    public:
        AbstractVideoProcessingModule();
        virtual ~AbstractVideoProcessingModule();

//        virtual bool setup() //setup sources
//        virtual void run();
//        virtual void send();
//        virtual int getFrame();
//        virtual void display();
    protected:
    private:
        std::vector<IplImage * > m_imageBuffer; // dynamical buffer of stored images
        std::vector<AbstractVideoSource * > m_sources; // dynamical array of sources, in ordrer to be modulable
        // AbstractCommunicationProtocol * m_protocol;
};

#endif // ABSTRACTVIDEOPROCESSINGMODULE_H
