#ifndef CAMERACALIBRATION_H_INCLUDED
#define CAMERACALIBRATION_H_INCLUDED

#include <opencv/cv.h>
#include <opencv/highgui.h>


#define nbImages 19
#define nbLines 6
#define nbColumns 8
#define nbCorners nbLines*nbColumns

#define offsetX 1
#define offsetY 1

#define ratioPixelOverUnit 3 // size of a single square = 3x3 (cm)
#define D 60 // ball real diameter (mm)

// functions dealing with only one image
void compute_and_display_image_corners(char * imageName, CvSize * imageSize, CvSize chessboardSize, CvPoint2D32f * cornersArrayToFillIn);
void improve_precision(IplImage *image, CvPoint2D32f * cornersArray, int cornersCount);

// various matrices initialisations
void initiate_matrices_for_calibration(CvMat * intrinsicMatrix, CvMat * distortionCoeffs,
                                       CvMat * nbTotalCorners, CvMat * objectRealCoordinates,
                                       CvMat * cornersMat, CvPoint2D32f * cornersList);

// debug
void display_CvPoint2D32f(CvPoint2D32f * arrayToDisplay, int arraySize);
void display_CvMat(CvMat * arrayToDisplay, int arraySize);

#endif // CAMERACALIBRATION_H_INCLUDED
