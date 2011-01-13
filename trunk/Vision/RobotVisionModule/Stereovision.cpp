#include "Stereovision.hpp"

//ctor
Stereovision::Stereovision():m_LeftCamera(NULL),m_RightCamera(NULL)
{
}

//dtor
Stereovision::~Stereovision()
{
    delete m_LeftCamera;
    delete m_RightCamera;
}

// connect sources, external..
void Stereovision::Setup()
{
    try{
        // setup left camera
        m_LeftCamera = new Camera(0);
        if(!m_LeftCamera->isOpened())
           throw (new std::exception());

        // setup right camera
        m_RightCamera = new Camera(1);
        if(!m_RightCamera->isOpened())
            throw (new std::exception());

    }
    catch(std::exception e){
        cerr<<e.what(); exit(1);
    }
}


// display both cameras untouched
void Stereovision::RawVideoDisplay()
{
    cv::Mat frame;
    cv::namedWindow( "rawDisplay_Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "rawDisplay_Right", CV_WINDOW_AUTOSIZE);
    char c = 0;

    while(c<1)
    {
        c = (char)cv::waitKey(2); // shortest possible delay(experience result)

        // left
        *m_LeftCamera >> frame;

        if(frame.empty()) break;

        cv::imshow( "rawDisplay_Left", frame );

        // right
        *m_RightCamera >> frame;

        if(frame.empty())break;

        cv::imshow( "rawDisplay_Right", frame );
    }
}

// main routine of the video processing module
void Stereovision::Run()
{
    IplImage* img;
    CvCapture * capture;
    capture = cvCreateFileCapture("test.avi");

     if( !capture )
        exit(1);

    cvNamedWindow("video", CV_WINDOW_AUTOSIZE);
    char key = 0;

    // On essaye de capturer une frame
    if(!cvGrabFrame( capture ))
        exit(1);

    // Tant qu'on n'a pas appuyé sur Q, on continue :
    while(key != 'q')
    {
        // On décompresse la dernière frame récupérée...
        img = cvRetrieveFrame( capture );

        // ... et on la montre à l'écran.
        cvShowImage( "video", img );

        // On attend 10 ms : IMPORTANT !
        key = cvWaitKey(10);

        // On essaye de capturer la frame suivante
        if(!cvGrabFrame( capture ))
            key = 'q';
    }

    // On détruit les fenêtres créées : nouvelle fonction. ;)
    cvDestroyAllWindows();

    // On n'a pas besoin de détruire l'image.
    img = NULL;

    // On détruit la capture
    cvReleaseCapture( &capture );

}
