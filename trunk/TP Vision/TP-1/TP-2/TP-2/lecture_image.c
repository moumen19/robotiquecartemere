////////////////////////////////////////////////////////////////////////
//
// hello-world.cpp
//
// This is a simple, introductory OpenCV program. The program reads an
// image from a file and displays the result.
//
////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>


int main(int argc, char *argv[])
{
  IplImage* img[1];
  int height,width,step,channels;
  uchar *data;
  int i,j,k;

  if(argc<2)
  {
    printf("Usage: main <image-file-name>\n\7");
    exit(0);
  }

  // load an image
  img[0]=cvLoadImage(argv[1],-1);
  if(!img)
  {
    printf("Could not load image file: %s\n",argv[1]);
    exit(0);
  }

  // get the image data
  height    = img[0]->height;
  width     = img[0]->width;
  step      = img[0]->widthStep;
  channels  = img[0]->nChannels;

  data      = (uchar *)img[0]->imageData;
  printf("Processing a %dx%d image with %d channels\n",height,width,channels);
  printf( "Image size: %d bytes\n", img[0]->imageSize );
  printf( "Width step: %d bytes\n", img[0]->widthStep );



  CvSize pattern_size;
  pattern_size = cvSize(8,6);
  CvPoint2D32f corners[48];
  int* cornerCount=NULL;
  int patternWasFound;

  patternWasFound=cvFindChessboardCorners( img[0], pattern_size, corners, &cornerCount, CV_CALIB_CB_ADAPTIVE_THRESH);
  printf("%d\nnombre de coin %d\n",patternWasFound,cornerCount);

  cvDrawChessboardCorners(img[0],pattern_size, corners, cornerCount, patternWasFound);




  // create a window
  cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
  cvMoveWindow("mainWin", 100, 100);

    // show the image
  cvShowImage("mainWin", img[0] );

  // wait for a key
  cvWaitKey(0);

  // release the image
  cvReleaseImage(&img[0] );
  return 0;
}
