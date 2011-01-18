#ifndef STEREOVISION_H_INCLUDED
#define STEREOVISION_H_INCLUDED

#include <stdio.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

// global but only for this file
CvPoint pointPixelCoordinates;

// manually select homolog
CvPoint get_one_pixel(char * imageFileName);
void stereoMouseEventHandler(int event, int x, int y, int flags, void *param);

// from Matlab
int  cvStereoTriangulation(CvPoint P1, CvPoint P2, int display);

#endif // STEREOVISION_H_INCLUDED
