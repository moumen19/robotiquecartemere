
// Strongly inspired from:
// http://www.siteduzero.com/tutoriel-3-146397-tp-1-la-base-de-notre-application.html

#ifndef ABSTRACTVIDEOSOURCE_H
#define ABSTRACTVIDEOSOURCE_H

// to remove: stderr to be replace by exceptions
#include <stdio.h>

#include <string>

#include <opencv/cv.h>
#include <opencv/highgui.h>
//#include <opencv/cvaux.h>

class AbstractVideoSource
{
    public:
        virtual ~AbstractVideoSource();

        // manage video stream
        virtual void open() = 0;
        virtual void close() = 0;

        // get some informations
        virtual std::string getInfos() { return m_infos;}

        // retrieve an image from the source
        virtual cv::Mat getFrame() = 0;

    protected:
        std::string m_infos;
    private:
};

#endif // ABSTRACTVIDEOSOURCE_H
