#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main()
{

IplImage* srcLeft = cvLoadImage("/home/olivier/projet_vision/left1.jpg");
IplImage* srcRight = cvLoadImage("/home/olivier/projet_vision/right1.jpg");
IplImage* leftImage = cvCreateImage(cvGetSize(srcLeft), IPL_DEPTH_8U, 1);
IplImage* rightImage = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 1);
IplImage* disp = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_16S, 1);
CvStereoBMState* state = cvCreateStereoBMState( CV_STEREO_BM_BASIC, 0 );
cvCvtColor(srcLeft, leftImage, CV_BGR2GRAY);
cvCvtColor(srcRight, rightImage, CV_BGR2GRAY);

cvFindStereoCorrespondenceBM( leftImage, rightImage, disp, state);
/*
	int i,j;
	CvScalar pixel,k;
	CvSize size;
	size.width = disp->width;
	size.height = 50;
	IplImage* max_disp = cvCreateImage(size, IPL_DEPTH_16S, 1);
	k.val[0]=0;
	for(i=0; i< disp->width ; i++)
	{
		for( j=0 ; j< disp->height ; j++)
		{
			pixel = cvGet2D(disp,j,i);
			if((k.val[0] < pixel.val[0]) && (pixel.val[0] < 220))
			{
				k.val[0] = pixel.val[0];
			}
		}	
		for( j=0 ; j< 50 ; j++)
		{
			cvSet2D(max_disp,j,i,k);
		}
		k.val[0]=0;
	}
*/

CvMat temp;
CvSize size = cvGetSize(disp);
printf("plop\n");
CvMat* disparity = cvGetMat(disp,&temp,0,3);
printf("plop\n");
CvMat* disparity_visual = cvCreateMat( size.height, size.width, CV_8U );
printf("plop\n");
cvConvertScale( disparity, disparity_visual, -8 );
printf("plop\n");


cvNamedWindow("left", CV_WINDOW_AUTOSIZE);
cvShowImage( "left", leftImage);
cvNamedWindow("right", CV_WINDOW_AUTOSIZE);
cvShowImage( "right", rightImage);
//cvNamedWindow("disparite", CV_WINDOW_AUTOSIZE);
//cvShowImage( "disparite", disp);
//cvNamedWindow("max disp", CV_WINDOW_AUTOSIZE);
//cvShowImage( "max disp", max_disp);

cvNamedWindow("disp", CV_WINDOW_AUTOSIZE);
cvShowImage( "disp", disparity_visual);
cvWaitKey();
cvDestroyAllWindows();


return 0;
}
