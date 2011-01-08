/**
 * Display video from webcam
 *
 */

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main( int argc, char **argv )
{
    CvCapture *capture = 0;
    IplImage  *frame = 0;
    int       key = 0;

    /* initialize camera */
    capture = cvCaptureFromCAM( 0 );

    /* always check */
    if ( !capture ) {
        fprintf( stderr, "Cannot open initialize webcam!\n" );
        return 1;
    }

    /* create a window for the video */
    cvNamedWindow( "result", CV_WINDOW_NORMAL );

    while( key != 'q's ) {
		/* get a frame */
        frame = cvQueryFrame( capture );

        /* always check */
        if( !frame ) break;

        /* display current frame */
        cvShowImage( "result", frame );

        /* exit if user press 'q' */
        key = cvWaitKey( 10 );
    }

    /* free memory */
    cvDestroyWindow( "result" );
    cvReleaseCapture( &capture );

    return 0;
}


