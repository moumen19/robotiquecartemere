////////////////////////////////////////////////////////////////////////
//
// hello-world.cpp
//
// This is a simple, introductory OpenCV program. The program reads an
// image from a file and displays the result.
//
////////////////////////////////////////////////////////////////////////
#include "histogramme.h"

// D:\TRAVAIL\M2\Robotique Avancée\Vision\TPs\TP_Vision_1\bin\Debug\TP_Vision_1.exe test.jpg lena.jpg

int main(int argc, char *argv[])
{

  // verification des arguments en ligne de commande
  if(argc<2)
  {
    printf("1 images a mettre en argument");
    exit(0);
  }

  IplImage* img[2];
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



  // create a black and white image
  greylevel( &img[0]);

  reduce_greylevel( &img[0], 16);


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
