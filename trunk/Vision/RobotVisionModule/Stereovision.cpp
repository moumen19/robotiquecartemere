#include "Stereovision.hpp"

//ctor
Stereovision::Stereovision()
{

}

//dtor
Stereovision::~Stereovision()
{

}

bool Stereovision::setup()
{
    // connect sources
    // connect external...
    return true;
}

void Stereovision::send()
{
}
cv::Mat Stereovision::getFrame()
{
    return cv::Mat();
}
void Stereovision::display()
{
}
// main routine of the video processing module
void Stereovision::run()
{

   // try{ // the grab of a frame can fail as it is not secured inside the video source classes
    //    getFrame

////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
    /* initialize camera
     cv::VideoCapture capture = VideoCapture(0);
    get a frame:
		equivalent for  frame = cvQueryFrame(capture) previously in C
        capture >> frame;
    */

    //IplImage  *frame = 0;
    CvCapture *capture = 0;
    IplImage  *frame = 0;
    int       key = 0;

    /* initialize camera */
    capture = cvCaptureFromCAM( -1 );


    /* always check */
    if ( !capture ) {
        fprintf( stderr, "Cannot open initialize webcam!\n" );
        exit(1);
    }

     /* create a window for the video */
    cvNamedWindow( "result", CV_WINDOW_AUTOSIZE);

    while( key <= 0 ) {

        frame = cvQueryFrame(capture);

        /* always check */
        if( !frame ) break;

        /* display current frame */
        cvShowImage( "result", frame );

        /* exit if user press 'q' */
        key = cvWaitKey( 2 );
    }

    /* free memory */
    cvDestroyWindow( "result" );
    cvReleaseCapture( &capture );
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
}
