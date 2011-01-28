#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int main()
{

//IplImage* srcLeft = cvLoadImage("/home/olivier/projet_vision/left2.jpg");
//IplImage* srcRight = cvLoadImage("/home/olivier/projet_vision/right2.jpg");

//////////////////////////////////

   IplImage* srcLeft;
   IplImage* srcRight;

    // Si vous voulez lire une webcam à la place :
    CvCapture* capture = cvCreateCameraCapture( 1 );
    CvCapture* capture2 = cvCreateCameraCapture( -1 );

    // On vérifie que la capture est bien initialisée,
    // si ce n'est pas le cas, on quitte le programme.
    if( !capture ) 
        return 10;
    if( !capture2 ) 
        return 11;

    cvNamedWindow("video", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("video2", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("disparite", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("max_disp", CV_WINDOW_AUTOSIZE);
    char key;

    // On essaye de capturer une frame
    if(!cvGrabFrame( capture ))
        return 20;
    if(!cvGrabFrame( capture2 ))
        return 21;

     // Tant qu'on n'a pas appuyé sur Q, on continue :
    while(key != 'q')
    {
        // On décompresse la dernière frame récupérée...
        srcLeft = cvRetrieveFrame( capture );
	srcRight = cvRetrieveFrame( capture2 );

//////////////////////////////////////

IplImage* leftImage = cvCreateImage(cvGetSize(srcLeft), IPL_DEPTH_8U, 1);
IplImage* rightImage = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 1);
IplImage* disp = cvCreateImage(cvGetSize(srcRight), IPL_DEPTH_8U, 1);
cvCvtColor(srcLeft, leftImage, CV_BGR2GRAY);
cvCvtColor(srcRight, rightImage, CV_BGR2GRAY);

//cvFindStereoCorrespondence( leftImage, rightImage, CV_DISPARITY_BIRCHFIELD, disp, 50, 15, 3, 6, 8, 15 );

cvFindStereoCorrespondence( 
	leftImage, // left image grayscale 8-bit
	rightImage, //right image grayscale 8-bit
	CV_DISPARITY_BIRCHFIELD, // Algorithm used to find a disparity
	disp, // grayscale 8-bit image that codes the scaled disparity
	50, // Maximum possible disparity (Too big values slow down the process significantly)
	4, // constant occlusion penalty
	5, // conscvShowImage( "disparite", disp);tant match reward
	30, // defines a highly reliable region (set of contiguous pixels whose reliability is at least this value)
	15, // defines a moderately reliable region
	20 // defines a slightly reliable region
	);
/*
	int i,j;
	CvScalar pixel,k;
	for(i=0; i< disp->width ; i++)
	{
		for( j=0 ; j< disp->height ; j++)
		{
			pixel = cvGet2D(disp,j,i);
			k.val[0] = 6*pixel.val[0];
			cvSet2D(disp,j,i,k);
		}
	}
*/



	int i,j;
	CvScalar pixel,k;
	CvSize size;
	size.width = disp->width;
	size.height = 50;
	IplImage* max_disp = cvCreateImage(size, IPL_DEPTH_8U, 1);
	k.val[0]=0;
	for(i=0; i< disp->width ; i++)
	{
		for( j=0 ; j< disp->height ; j++)
		{
			pixel = cvGet2D(disp,j,i);
			if((k.val[0] < pixel.val[0]) && (pixel.val[0] < 240))
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


/////////////////////////////////////

        // ... et on la montre à l'écran.
        cvShowImage( "video", srcLeft );
	cvShowImage( "video2", srcRight );
	cvShowImage( "disparite", disp);
	cvShowImage( "max_disp", max_disp);

        // On attend 10 ms : IMPORTANT !
        key = cvWaitKey(10);
    
        // On essaye de capturer la frame suivante
        if(!cvGrabFrame( capture ))
            key = 'q';  

	if(!cvGrabFrame( capture2 ))
            key = 'q';
    }

   // cvWaitKey();

    // On détruit les fenêtres créées : nouvelle fonction. ;)  
    cvDestroyAllWindows();

    // On détruit la capture
    cvReleaseCapture( &capture );
    cvReleaseCapture( &capture2 );


return 0;
}
