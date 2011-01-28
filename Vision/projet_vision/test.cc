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
   // cvNamedWindow("max_disp", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("map", CV_WINDOW_AUTOSIZE);
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

IplImage* leftImageRec = cvCloneImage(leftImage);
IplImage* rightImageRec = cvCloneImage(rightImage);



	double _cameraL[9]= 
	{5.66399536e+02, 0, 3.07335602e+02,
        0, 5.67625793e+02, 2.60896210e+02,
        0, 0, 1 };
	double _cameraR[9]= 
	{5.68537720e+02, 0, 2.85776093e+02,
        0, 5.68358704e+02, 2.62164856e+02,
        0, 0, 1 };
        CvMat  cameraL = cvMat( 3, 3, CV_64F, _cameraL );
        CvMat  cameraR = cvMat( 3, 3, CV_64F, _cameraR );

        double _dist_coeffsL[4]= {1.56113012e-02,-5.93487453e-03,-3.65598616e-03,1.19160500e-03};
	double _dist_coeffsR[4]= {-4.34344308e-03,1.77513063e-01,2.47111614e-03,-1.57373734e-02};
        CvMat dist_coeffsL = cvMat( 1, 4, CV_64F, _dist_coeffsL );
        CvMat dist_coeffsR = cvMat( 1, 4, CV_64F, _dist_coeffsR );

/*
cvUndistort2(leftImage, leftImageRec, &cameraL, &dist_coeffsL);
cvUndistort2(rightImage, rightImageRec,  &cameraR,  &dist_coeffsR);
*/


///////////////////////////////////MORGAN////////////////////


double cvStereoCalibrate(
	const CvMat* objectPoints, 
	const CvMat* imagePoints1, 
	const CvMat* imagePoints2, 
	const CvMat* pointCounts, 
	CvMat* cameraMatrix1, 
	CvMat* distCoeffs1, 
	CvMat* cameraMatrix2, 
	CvMat* distCoeffs2, 
	CvSize imageSize, 
	CvMat* R, 
	CvMat* T, 
	CvMat* E=0, 
	CvMat* F=0, 
	CvTermCriteria term_crit=cvTermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 30, 1e-6), 
	int flags=CV_CALIB_FIX_INTRINSIC
);

//////////////////////////////////////////////////////////


// The output  rectification transforms (rotation matrices) for the first and the second cameras, respectively.
	CvMat* R1; 
	CvMat* R2; 
// The output  projection matrices in the new (rectified) coordinate systems
	CvMat* P1; 
	CvMat* P2; 
// The output  disparity-to-depth mapping matrix
	CvMat* Q=0;

	//CvSize newImageSize=cvSize(0, 0);

cvStereoRectify(
	cameraMatrix1, // used in cvStereoCalibrate
	cameraMatrix2, // used in cvStereoCalibrate
	distCoeffs1, // used in cvStereoCalibrate
	distCoeffs2, // used in cvStereoCalibrate
	imageSize, // used in cvStereoCalibrate
	R, // used in cvStereoCalibrate
	T, // used in cvStereoCalibrate
	R1, 
	R2, 
	P1, 
	P2, 
	Q=0, 
	CV_CALIB_ZERO_DISPARITY, 
/*	-1, 
	newImageSize, 
	CvRect* roi1=0, 
	CvRect* roi2=0*/
);

///////////////////////////////////////////////////////////

void cvInitUndistortRectifyMap(
	const CvMat* cameraMatrix, 
	const CvMat* distCoeffs, 
	const CvMat* R, 
	const CvMat* newCameraMatrix, 
	CvArr* map1, 
	CvArr* map2
);


//////////////////////////////////////////////////////////


/*
	cvShowImage( "video", leftImage );
	cvShowImage( "video2", rightImage );
	cvShowImage( "disparite", leftImageRec );
	cvShowImage( "max_disp", rightImageRec );
	cvWaitKey();

*/



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

	size.width = disp->width;
	size.height = 255;
	IplImage* map = cvCreateImage(size, IPL_DEPTH_8U, 1);
	k.val[0]=255;
	for(i=0; i< map->width ; i++)
	{
		for( j=0 ; j< map->height ; j++)
		{
			cvSet2D(map,j,i,k);
		}	
	}
	k.val[0]=0;
	int l;
	for(i=0; i< map->width ; i++)
	{
		pixel = cvGet2D(max_disp,0,i);
		l=pixel.val[0]*2;
		if(l>255) l=255;
		cvSet2D(map,l,i,k);
		cvSet2D(map,l+1,i,k);
		cvSet2D(map,l-1,i,k);	
	}


/////////////////////////////////////

        // ... et on la montre à l'écran.
        cvShowImage( "video", srcLeft );
	cvShowImage( "video2", srcRight );
	cvShowImage( "disparite", disp);
	//cvShowImage( "max_disp", max_disp);
	cvShowImage( "map", map);

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
