#ifndef HISTOGRAMS_H_INCLUDED
#define HISTOGRAMS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <cxcore.h>


void compute_histo_16(char * img_name, int * histo_16xRGB);

void compute_histo_256(char * img_name);

int intersection_histo(int * histo1, int * histo2, int histoSize);

void greylevel(IplImage* img[0]);

void reduce_greylevel(IplImage* img[0], int level);

#endif // HISTOGRAMS_H_INCLUDED
