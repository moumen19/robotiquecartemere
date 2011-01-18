#ifndef COLORSEGMENTATION_H_INCLUDED
#define COLORSEGMENTATION_H_INCLUDED


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>


#define red 0
#define green 1
#define blue 2

#define redHue 0
#define greenHue 60
#define blueHue 120

// Global variables: mendatory for trackbars callback functions
IplImage * imgBGR;
IplImage * imgHSV;
IplImage * grayMaskHSV;
IplImage * improvedMaskOnBGR;

int Hmin;
int Hmax;
int Smin;
int Smax;
int maskInversion;
int Vmin;
int Vmax;

// Used to Determine color segmentation thresholds with trackbars
void segment_HSVimage(void);

// event handlers
void refresh_hue_min(int changedValue);
void refresh_hue_max(int changedValue);
void refresh_saturation_min(int changedValue);
void refresh_saturation_max(int changedValue);
void refresh_value_min(int changedValue);
void refresh_value_max(int changedValue);
void refresh_inversion(int changedValue);
void colorMouseEventHandler(int event, int x, int y, int flags, void *param);

// checking focals
void get_balls_pixelsSizes(int * ballsPixelsSizes, char * fileToAnalyse);

// various functions
void improve_and_apply_grayScaleMask(IplImage ** mask, IplImage * img, IplImage ** appliedMask);
int pixelSize_of_binary_blob(IplImage * binaryImage);

#endif // COLORSEGMENTATION_H_INCLUDED
