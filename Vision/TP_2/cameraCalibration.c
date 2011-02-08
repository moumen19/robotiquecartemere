

#include <stdlib.h>
#include <stdio.h>

#include "cameraCalibration.h"


void compute_and_display_image_corners(char * imageName, CvSize * imageSize, CvSize chessboardSize, CvPoint2D32f * cornersArrayToFillIn)
{
    IplImage * img = 0;
    int cornersCount = 0;
    int patternWasFound = 0;
    int i;


    img = cvLoadImage(imageName, 1);

    *imageSize = cvGetSize(img); // useful only for calibration function

    //initialisation of the given array
    for(i=0;i<chessboardSize.height*chessboardSize.width;i++){ cornersArrayToFillIn[i].x= 0; cornersArrayToFillIn[i].y= 0; }

    printf("OK1\n");
    // core algorithm
    patternWasFound = cvFindChessboardCorners(img, chessboardSize, cornersArrayToFillIn, &cornersCount, 0);

    printf("OK2\n");

    // display_array_values(cornersArrayToFillIn,chessboardSize.height*chessboardSize.width);
    improve_precision(img, cornersArrayToFillIn, cornersCount);
    // display_array_values(cornersArrayToFillIn,chessboardSize.height*chessboardSize.width);

    // visual only part

    cvDrawChessboardCorners(img, chessboardSize, cornersArrayToFillIn, cornersCount, patternWasFound);
    cvNamedWindow(imageName, CV_WINDOW_AUTOSIZE);
    cvMoveWindow(imageName, 100, 100);
    cvShowImage(imageName, img );
    cvWaitKey(200);
    cvDestroyWindow(imageName);


    // end
    cvReleaseImage(&img );

}


void improve_precision(IplImage *image, CvPoint2D32f * cornersArray, int cornersCount)
{
  // subPix tool requires a gray scale:
  IplImage * gray_image = cvCreateImage(cvSize(image->width,image->height), 8, 1);
  cvCvtColor(image, gray_image, CV_BGR2GRAY);

  // we use 2 different terminal criteria: nomber of iterations and/or precision (first to be reached)
  cvFindCornerSubPix(gray_image, cornersArray, cornersCount,cvSize(11,11),cvSize(-1,-1), cvTermCriteria(CV_TERMCRIT_EPS +CV_TERMCRIT_ITER, 30, 0.1 ));

}



void initiate_matrices_for_calibration(CvMat * intrinsicMatrix, CvMat * distortionCoeffs,
                                       CvMat * nbTotalCorners, CvMat * objectRealCoordinates,
                                       CvMat * cornersMat, CvPoint2D32f * cornersList)
{
    int i,j,t;

    /*////////////// Initialisation of intrinsic matrix //////////////////////////
    Focal lengths set to 1 => corresponds to the camera's aspect (normal) ratio
    Matrix aspect:
    [fx 0  Cx]
    [0  fy Cy]
    [0  0   1] */
    cvmSet(intrinsicMatrix,0,0 , 1); //fx
    cvmSet(intrinsicMatrix,1,1 , 1); //fy

    /*/////////////   CvMat * possible accessors  //////////////////////////
    => Float Matrix only:
    void cvmSet( CvMat* mat, int row, int col, double value )
    double cvmGet( const CvMat* mat, int row, int col )
    void cvSetReal2D(CvArr* arr, int idx0, int idx1, double value)
    double cvGetReal2D(CvArr* arr, int idx0, int idx1)

    => All:
    CV_MAT_ELEM( *data, double, x, y) = 1     */

    /*/////////////// Initialisation of distortion coeffs ////////////////////////*/
    cvmSet(distortionCoeffs,0,0,  0); // k1
    cvmSet(distortionCoeffs,1,0,  0); // k2
    cvmSet(distortionCoeffs,2,0,  0); // p1
    cvmSet(distortionCoeffs,3,0,  0); // p2
    // cvmSet(distortionCoeffs,4,0,  0); // k3

	for (t=0;t<nbImages;t++) {
		for (j=0;j<nbLines;j++) {   // 3D-matrives stored in 1D-ones (index: columnN° + offset1*LineN° + offset2*imageN°)
			for (i=0;i<nbColumns;i++) {

                /*/////////////// Description of the real chessboard matrix ////////////////////////
                Here we initialise the pattern object's real 2D-coordinates, specified in the chessboard plan
                Coordinates of a 6x8 chessboard with square of 1[unit] for a single image:
                [(0;0) (0;1) (0;2) ... (0;7)]
                [(1;0) (1;1) (1;2) ... (1;7)]
                [       ...                 ]
                [(4;0) (4;1) (4;2) ... (4;7)]
                That implies the calibration result will be in [unit] too  */
				cvmSet(objectRealCoordinates, t*nbCorners +j*nbColumns +i , 0 , j *ratioPixelOverUnit); // x (squares of 3x3cm)
				cvmSet(objectRealCoordinates, t*nbCorners +j*nbColumns +i , 1 , i *ratioPixelOverUnit); // y
				cvmSet(objectRealCoordinates, t*nbCorners +j*nbColumns +i , 2 , 0); // z=0

                /*///////// Conversion from CvPoint2D32f to CvMat for corners coordinates //////////////*/
                cvmSet(cornersMat, t*nbCorners +j*nbColumns +i , 0 , cornersList[t*nbCorners +j*nbColumns +i].x);
                cvmSet(cornersMat, t*nbCorners +j*nbColumns +i , 1 , cornersList[t*nbCorners +j*nbColumns +i].y);

                //printf("(%.1f;%.1f)\t",cvmGet(cornersMat_Left,t*nbImages +j*nbColumns +i,0),cvmGet(cornersMat_Left,t*nbImages +j*nbColumns +i,1));
			}
		}
		CV_MAT_ELEM( *nbTotalCorners, int, t, 0)= nbCorners;  // save nb of corners for each image
	}
}


// debugging tools
void display_CvPoint2D32f(CvPoint2D32f * arrayToDisplay, int arraySize)
{
    int i;
    printf("\n\n\tCvPoint2D32f Coordinates (x;y)\n\n");
    for(i=0;i<arraySize;i++)
    {
        printf("%.3f;%.3f\n",arrayToDisplay[i].x,arrayToDisplay[i].y);
    }
}

void display_CvMat(CvMat * arrayToDisplay, int arraySize)
{
    int i;
    printf("\n\n\tCvMat Coordinates (x;y)\n\n");
    for(i=0;i<arraySize;i++)
    {
        printf("(%.1f;%.1f)\t",cvmGet(arrayToDisplay,i,0),cvmGet(arrayToDisplay,i,1));
    }
}


