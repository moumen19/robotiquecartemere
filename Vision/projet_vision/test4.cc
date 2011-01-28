#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


using namespace std;


double StereoCalib(IplImage* imageArray[MAX_CAMERA][MAX_BUFFER], int nx, int ny, int useUncalibrated, 
				   CvMat * R_, CvMat * T_, CvMat * F_, 
				   CvMat * P0, CvMat * P1, CvMat * M1_, CvMat* M2_, CvMat *D1_, CvMat *D2_,
				   CvMat * mx1, CvMat * my1, CvMat * mx2, CvMat * my2, CvMat * Q)
{
    bool isVerticalStereo = false;
	int displayCorners = 1;
    int showUndistorted = 1;
    const int maxScale = 1;
    const float squareSize = .03; //Set this to your actual square size
    int i, j, nframes, n = nx*ny, N = 0;
    vector<string> imageNames[2];
    vector<CvPoint3D32f> objectPoints;
    vector<CvPoint2D32f> points[2];
    vector<int> npoints;
    vector<uchar> active[2];
    vector<CvPoint2D32f> temp_0(n);
    vector<CvPoint2D32f> temp_1(n);

	int result_prev=0;
	int good = 0;
    CvSize imageSize = {0,0};
    // ARRAY AND VECTOR STORAGE:
    double M1[3][3], M2[3][3]; //D1[5], D2[5];
    double E[3][3];//, R[3][3], T[3],  F[3][3];
    //CvMat M1_ = cvMat(3, 3, CV_64F, M1 );
    //CvMat M2_ = cvMat(3, 3, CV_64F, M2 );
    //CvMat D0_ = cvMat(1, 5, CV_64F);
    //CvMat D1_ = cvMat(1, 5, CV_64F);
   // CvMat R_ = cvMat(3, 3, CV_64F, R );
    //CvMat T_ = cvMat(3, 1, CV_64F, T );
    CvMat E_ = cvMat(3, 3, CV_64F, E );
    //CvMat F_ = cvMat(3, 3, CV_64F, F );
	if( displayCorners ){
        cvNamedWindow( "left corners", 1 );
		cvNamedWindow( "right corners", 1 );
	}


// READ IN THE LIST OF CHESSBOARDS:

    for(i=0;i<MAX_BUFFER;i++)
    {
        int count_0 = 0, result_0=0;
        vector<CvPoint2D32f>& pts_0 = points[0];
        IplImage* img_0 = cvCloneImage( imageArray[0][i] );

        if( !img_0 )
            break;
        int count_1 = 0, result_1=0;
        vector<CvPoint2D32f>& pts_1 = points[1];
        IplImage* img_1 = cvCloneImage( imageArray[1][i] );

        if( !img_1 )
            break;

        imageSize = cvGetSize(img_0);

	//GET NEXT IMAGE IN ARRAY

    //FIND CHESSBOARDS AND CORNERS THEREIN:
        for( int s = 1; s <= maxScale; s++ )
        {
            IplImage* timg_0 = img_0;
            if( s > 1 )
            {
                timg_0 = cvCreateImage(cvSize(img_0->width*s,img_0->height*s),
                    img_0->depth, img_0->nChannels );
                cvResize( img_0, timg_0, CV_INTER_CUBIC );
            }
            result_0 = cvFindChessboardCorners( timg_0, cvSize(nx, ny),
                &temp_0[0], &count_0,
                CV_CALIB_CB_ADAPTIVE_THRESH |
                CV_CALIB_CB_NORMALIZE_IMAGE);

            if( timg_0 != img_0 )
                cvReleaseImage( &timg_0 );

            if( result_0 || s == maxScale )
                for( j = 0; j < count_0; j++ )
            {
                temp_0[j].x /= s;
                temp_0[j].y /= s;
            }
            if( result_0 )
                break;
		}
		for( int s = 1; s <= maxScale; s++ )
        {
            IplImage* timg_1 = img_1;
            if( s > 1 )
            {

                timg_1 = cvCreateImage(cvSize(img_1->width*s,img_1->height*s),
                    img_1->depth, img_1->nChannels );
                cvResize( img_1, timg_1, CV_INTER_CUBIC );
            }

            result_1 = cvFindChessboardCorners( timg_1, cvSize(nx, ny),
                &temp_1[0], &count_1,
                CV_CALIB_CB_ADAPTIVE_THRESH |
                CV_CALIB_CB_NORMALIZE_IMAGE);

            if( timg_1 != img_1 )
                cvReleaseImage( &timg_1 );

            if( result_1 || s == maxScale )
                for( j = 0; j < count_0; j++ )
            {
                temp_1[j].x /= s;
                temp_1[j].y /= s;
            }
            if( result_1 )
                break;
		}
        if( displayCorners )
        {
            //printf("%s\n", buf);
            IplImage* cimg_0 = cvCreateImage( imageSize, 8, 3 );
            IplImage* cimg_1 = cvCreateImage( imageSize, 8, 3 );
            cvCvtColor( img_0, cimg_0, CV_GRAY2BGR );
            cvCvtColor( img_1, cimg_1, CV_GRAY2BGR );
            cvDrawChessboardCorners( cimg_0, cvSize(nx, ny), &temp_0[0], count_0, result_0 );
            cvDrawChessboardCorners( cimg_1, cvSize(nx, ny), &temp_1[0], count_1, result_1 );
            cvShowImage( "left corners", cimg_0 );
			cvShowImage( "right corners", cimg_1 );
            cvReleaseImage( &cimg_0 );
            cvReleaseImage( &cimg_1 );
            int c = cvWaitKey(10);
            if( c == 27 || c == 'q' || c == 'Q' ) //Allow ESC to quit
                exit(-1);
        }

        if( result_1 && result_0 )
        {
			N = pts_0.size();
			pts_0.resize(N + n, cvPoint2D32f(0,0));
			active[0].push_back((uchar)result_0);
		//assert( result != 0 );
         //Calibration will suffer without subpixel interpolation
            cvFindCornerSubPix( img_0, &temp_0[0], count_0,
                cvSize(11, 11), cvSize(-1,-1),
                cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,
                30, 0.01) );
            copy( temp_0.begin(), temp_0.end(), pts_0.begin() + N );

			//N = pts_0.size();
			pts_1.resize(N + n, cvPoint2D32f(0,0));
			active[1].push_back((uchar)result_1);
		//assert( result != 0 );
         //Calibration will suffer without subpixel interpolation
            cvFindCornerSubPix( img_1, &temp_1[0], count_1,
                cvSize(11, 11), cvSize(-1,-1),
                cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,
                30, 0.01) );
            copy( temp_1.begin(), temp_1.end(), pts_1.begin() + N );


			good++;
        }
		
        cvReleaseImage( &img_0 );
        cvReleaseImage( &img_1);
    }
    
    //printf("\n");
// HARVEST CHESSBOARD 3D OBJECT POINT LIST:
    nframes = good;//active[0].size();//Number of good chessboards found
    objectPoints.resize(nframes*n);
    for( i = 0; i < ny; i++ )
        for( j = 0; j < nx; j++ )
        objectPoints[i*nx + j] =
        cvPoint3D32f(i*squareSize, j*squareSize, 0);
    for( i = 1; i < nframes; i++ )
        copy( objectPoints.begin(), objectPoints.begin() + n,
        objectPoints.begin() + i*n );
    npoints.resize(nframes,n);
    N = nframes*n;
    CvMat _objectPoints = cvMat(1, N, CV_32FC3, &objectPoints[0] );
    CvMat _imagePoints1 = cvMat(1, N, CV_32FC2, &points[0][0] );
    CvMat _imagePoints2 = cvMat(1, N, CV_32FC2, &points[1][0] );
    CvMat _npoints = cvMat(1, npoints.size(), CV_32S, &npoints[0] );
    cvSetIdentity(M1_);
    cvSetIdentity(M2_);
    cvZero(D1_);
    cvZero(D2_);
	/*
		cvSet2D(&M1_,0, 0, cvRealScalar(431.78036));
		cvSet2D(&M1_,0, 1, cvRealScalar(0));
		cvSet2D(&M1_,0, 2, cvRealScalar(198.61));//WarpedFrame->width/3));
		cvSet2D(&M1_,1, 0, cvRealScalar(0));
		cvSet2D(&M1_,1, 1, cvRealScalar(447.4886));
		cvSet2D(&M1_,1, 2, cvRealScalar(132.724));//WarpedFrame->height/3));
		cvSet2D(&M1_,2, 0, cvRealScalar(0));
		cvSet2D(&M1_,2, 1, cvRealScalar(0));
		cvSet2D(&M1_,2, 2, cvRealScalar(1));

		cvSet2D(&M2_,0, 0, cvRealScalar(431.78036));
		cvSet2D(&M2_,0, 1, cvRealScalar(0));
		cvSet2D(&M2_,0, 2, cvRealScalar(198.61));//WarpedFrame->width/3));
		cvSet2D(&M2_,1, 0, cvRealScalar(0));
		cvSet2D(&M2_,1, 1, cvRealScalar(447.4886));
		cvSet2D(&M2_,1, 2, cvRealScalar(132.724));//WarpedFrame->height/3));
		cvSet2D(&M2_,2, 0, cvRealScalar(0));
		cvSet2D(&M2_,2, 1, cvRealScalar(0));
		cvSet2D(&M2_,2, 2, cvRealScalar(1));

		cvSet2D(&D1_,0, 1, cvRealScalar(0.702));
		cvSet2D(&D1_,0, 2, cvRealScalar(-8.6));
		cvSet2D(&D1_,0, 3, cvRealScalar(6.75));
		cvSet2D(&D1_,0, 4, cvRealScalar(0.0299));

		cvSet2D(&D2_,0, 1, cvRealScalar(0.702));
		cvSet2D(&D2_,0, 2, cvRealScalar(-8.6));
		cvSet2D(&D2_,0, 3, cvRealScalar(6.75));
		cvSet2D(&D2_,0, 4, cvRealScalar(0.0299));*/

// CALIBRATE THE STEREO CAMERAS

    fflush(stdout);
    cvStereoCalibrate( &_objectPoints, &_imagePoints2,
        &_imagePoints1, &_npoints,
        M1_, D1_, M2_, D2_,
        imageSize, R_, T_, &E_, F_,
        cvTermCriteria(CV_TERMCRIT_ITER+
        CV_TERMCRIT_EPS, 100, 1e-5),
        /*CV_CALIB_FIX_INTRINSIC + */CV_CALIB_FIX_ASPECT_RATIO +
        CV_CALIB_ZERO_TANGENT_DIST +
        CV_CALIB_SAME_FOCAL_LENGTH);


	for(int i=0; i<3;i++)
		for(int j=0; j<3; j++)
		{
			cvmSet(P0,i,j,cvmGet(R_,i,j));
		}
	for(int i=0; i<3; i++)
		cvmSet(P0,i,3,0);
	cvMatMul(M1_, P0, P0);

	for(int i=0; i<3;i++)
		for(int j=0; j<3; j++)
		{
			cvmSet(P1,i,j,cvmGet(R_,i,j));
		}
	for(int i=0; i<3; i++)
		cvmSet(P1,i,3,cvmGet(T_,i,0));
	cvMatMul(M2_, P1, P1);


	/*printf("Camera Matrix 1:\n");
	printMatrix(&M1_);
	printf("Camera Matrix 2:\n");
	printMatrix(&M2_);
	printf("Rotation:\n");
	printMatrix(R_);
	printf("Translation:\n");
	printMatrix(T_);
	printf("Fundamental Matrix:\n");
	printMatrix(F_);
*/
// CALIBRATION QUALITY CHECK

    vector<CvPoint3D32f> lines[2];
    points[0].resize(N);
    points[1].resize(N);
    _imagePoints1 = cvMat(1, N, CV_32FC2, &points[0][0] );
    _imagePoints2 = cvMat(1, N, CV_32FC2, &points[1][0] );
    lines[0].resize(N);
    lines[1].resize(N);
    CvMat _L1 = cvMat(1, N, CV_32FC3, &lines[0][0]);
    CvMat _L2 = cvMat(1, N, CV_32FC3, &lines[1][0]);

    cvUndistortPoints( &_imagePoints1, &_imagePoints1,
        M1_, D1_, 0, M1_ );
    cvUndistortPoints( &_imagePoints2, &_imagePoints2,
        M2_, D2_, 0, M2_ );
    cvComputeCorrespondEpilines( &_imagePoints1, 1, F_, &_L1 );
    cvComputeCorrespondEpilines( &_imagePoints2, 2, F_, &_L2 );

	double avgErr = 0;
    for( i = 0; i < N; i++ )
    {
        double err = fabs(points[0][i].x*lines[1][i].x +
            points[0][i].y*lines[1][i].y + lines[1][i].z)
            + fabs(points[1][i].x*lines[0][i].x +
            points[1][i].y*lines[0][i].y + lines[0][i].z);
        avgErr += err;
    }

    //printf( "avg err = %g\n", avgErr/(nframes*n) );

	cvDestroyWindow("left corners");
	cvDestroyWindow("right corners");

//COMPUTE AND DISPLAY RECTIFICATION
    if( showUndistorted )
	{
        /*CvMat* mx1 = cvCreateMat( imageSize.height,
            imageSize.width, CV_32F );
        CvMat* my1 = cvCreateMat( imageSize.height,
            imageSize.width, CV_32F );
        CvMat* mx2 = cvCreateMat( imageSize.height,

            imageSize.width, CV_32F );
        CvMat* my2 = cvCreateMat( imageSize.height,
            imageSize.width, CV_32F );*/
        CvMat* img1r = cvCreateMat( imageSize.height,
            imageSize.width, CV_8U );
        CvMat* img2r = cvCreateMat( imageSize.height,
            imageSize.width, CV_8U );
        CvMat* disp = cvCreateMat( imageSize.height,
            imageSize.width, CV_16S );
        CvMat* vdisp = cvCreateMat( imageSize.height,
            imageSize.width, CV_8U );
        CvMat* pair;
        double R1[3][3], R2[3][3];// P1[3][4], P2[3][4];
        CvMat R1_ = cvMat(3, 3, CV_64F, R1);
        CvMat R2_ = cvMat(3, 3, CV_64F, R2);
// IF BY CALIBRATED (BOUGUET'S METHOD)
        if( useUncalibrated == 0 )
        {
            //CvMat P1_ = cvMat(3, 4, CV_64F, P1);
            //CvMat P2_ = cvMat(3, 4, CV_64F, P2);
            cvStereoRectify( M1_, M2_, D1_, D2_, imageSize,
                R_, T_,
                &R1_, &R2_, P0, P1, Q,
                0 );
            //isVerticalStereo = fabs(P2[1][3]) > fabs(P2[0][3]);
    //Precompute maps for cvRemap()
            cvInitUndistortRectifyMap(M1_,D1_,&R1_,P0,mx1,my1);
            cvInitUndistortRectifyMap(M2_,D2_,&R2_,P1,mx2,my2);
        }
//OR ELSE HARTLEY'S METHOD
        else if( useUncalibrated == 1 || useUncalibrated == 2 )
     // use intrinsic parameters of each camera, but
     // compute the rectification transformation directly
     // from the fundamental matrix
        {
            double H1[3][3], H2[3][3], iM[3][3];
            CvMat _H1 = cvMat(3, 3, CV_64F, H1);
            CvMat _H2 = cvMat(3, 3, CV_64F, H2);
            CvMat _iM = cvMat(3, 3, CV_64F, iM);
    //Just to show you could have independently used F
            if( useUncalibrated == 2 )
                cvFindFundamentalMat( &_imagePoints1,
                &_imagePoints2, F_);
            cvStereoRectifyUncalibrated( &_imagePoints1,
                &_imagePoints2, F_,
                imageSize,
                &_H1, &_H2, 3);
            cvInvert(&M1_, &_iM);
            cvMatMul(&_H1, &M1_, &R1_);
            cvMatMul(&_iM, &R1_, &R1_);
            cvInvert(&M2_, &_iM);
            cvMatMul(&_H2, &M2_, &R2_);
            cvMatMul(&_iM, &R2_, &R2_);
    //Precompute map for cvRemap()
            cvInitUndistortRectifyMap(M1_,D1_,&R1_,M1_,mx1,my1);

            cvInitUndistortRectifyMap(M2_,D2_,&R2_,M2_,mx2,my2);
        }
        else
            assert(0);
        cvNamedWindow( "rectified", 1 );
// RECTIFY THE IMAGES AND FIND DISPARITY MAPS
		
        if( !isVerticalStereo )
            pair = cvCreateMat( imageSize.height, imageSize.width*2,
            CV_8UC3 );
        else
            pair = cvCreateMat( imageSize.height*2, imageSize.width,
            CV_8UC3 );
//Setup for finding stereo corrrespondences
        CvStereoBMState *BMState = cvCreateStereoBMState();
        assert(BMState != 0);
        BMState->preFilterSize=41;
        BMState->preFilterCap=31;
        BMState->SADWindowSize=41;
        BMState->minDisparity=-64;
        BMState->numberOfDisparities=128;
        BMState->textureThreshold=10;
        BMState->uniquenessRatio=15;
   for( i = 0; i < MAX_BUFFER; i++ )
        {
			
            IplImage* img1=cvCloneImage(imageArray[0][i]);
            IplImage* img2=cvCloneImage(imageArray[1][i]);
            if( img1 && img2 )
            {
                cvNamedWindow( "disparity" );
				CvMat part;
                cvRemap( img1, img1r, mx1, my1 );
                cvRemap( img2, img2r, mx2, my2 );
                
                    cvFindStereoCorrespondenceBM( img1r, img2r, disp,
                        BMState);
                    cvNormalize( disp, vdisp, 0, 256, CV_MINMAX );
                    
                    cvShowImage( "disparity", vdisp );
                
                if( !isVerticalStereo )
                {
                    cvGetCols( pair, &part, 0, imageSize.width );
                    cvCvtColor( img1r, &part, CV_GRAY2BGR );
                    cvGetCols( pair, &part, imageSize.width,
                        imageSize.width*2 );
                    cvCvtColor( img2r, &part, CV_GRAY2BGR );
                    for( j = 0; j < imageSize.height; j += 16 )
                        cvLine( pair, cvPoint(0,j),
                        cvPoint(imageSize.width*2,j),
                        CV_RGB(0,255,0));
                }

                cvShowImage( "rectified", pair );
                if( cvWaitKey(10) == 27 )
                    break;

			}
            cvReleaseImage( &img1 );
            cvReleaseImage( &img2 );
		}
        cvReleaseStereoBMState(&BMState);
        /*cvReleaseMat( &mx1 );
        cvReleaseMat( &my1 );
        cvReleaseMat( &mx2 );
        cvReleaseMat( &my2 );*/
        cvReleaseMat( &img1r );
        cvReleaseMat( &img2r );
        cvReleaseMat( &disp );
		cvDestroyWindow("disparity");
		cvDestroyWindow("rectified");
    }


	
	return avgErr/(nframes*n);	
}
