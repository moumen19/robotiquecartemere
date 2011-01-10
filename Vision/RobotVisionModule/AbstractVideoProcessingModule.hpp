#ifndef ABSTRACTVIDEOPROCESSINGMODULE_H
#define ABSTRACTVIDEOPROCESSINGMODULE_H

#include <vector>

#include "AbstractVideoSource.hpp"

class AbstractVideoProcessingModule
{
    public:
        virtual ~AbstractVideoProcessingModule();

        virtual bool setup() =0; //setup sources
        virtual void run() =0;
        virtual void send() =0;
        virtual cv::Mat getFrame() = 0;
        virtual void display() =0;
    protected:
    private:
        std::vector<cv::Mat * > m_imageBuffer; // dynamical buffer of stored images
        std::vector<AbstractVideoSource * > m_sources; // dynamical array of sources, in ordrer to be modulable
        // AbstractCommunicationProtocol * m_protocol;
};

#endif // ABSTRACTVIDEOPROCESSINGMODULE_H
