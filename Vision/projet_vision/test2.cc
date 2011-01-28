#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main()

{

IplImage* srcLeft = cvLoadImage("/home/olivier/projet_vision/left2.jpg",1);
IplImage* srcRight = cvLoadImage("/home/olivier/projet_vision/right2.jpg",1);
IplImage* leftImage = cvCreateImage(cvGetSize(srcLeft), IPL_DEPTH_8U, 1);
IplImage* rightImage = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 1);

cvCvtColor(srcLeft, leftImage, CV_BGR2GRAY);
cvCvtColor(srcRight, rightImage, CV_BGR2GRAY);

CvSize size = cvGetSize(srcLeft);

CvMat* disparity_left = cvCreateMat( size.height, size.width, CV_16S );
CvMat* disparity_right = cvCreateMat( size.height, size.width, CV_16S );

CvStereoGCState* state = cvCreateStereoGCState( 16, 2 );

cvFindStereoCorrespondenceGC( leftImage, rightImage,disparity_left, disparity_right, state, 0 );

cvReleaseStereoGCState( &state );
CvMat* disparity_left_visual = cvCreateMat( size.height, size.width, CV_8U );
cvConvertScale( disparity_left, disparity_left_visual, -16 );
/*
cvSaveImage("disparity.jpg", disparity_left_visual);
cvNamedWindow("win1",1);
cvNamedWindow("win2",1);
cvNamedWindow("win3",1); 
*/
for(;;)
{

cvShowImage("win1",leftImage);
cvShowImage("win2",rightImage);
cvShowImage("win3",disparity_left_visual); if(cvWaitKey(20)==27) break;

}

return 0;
}
 
