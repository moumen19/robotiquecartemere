
#include "histogramme.h"

void compute_histo_16(char * img_name, int * histo_16xRGB)
{
  // variables
  IplImage* img[1];
  int height,width,step,channels;
  uchar *data;
  int i,j,k;

  CvScalar pixel;
  int R=0;
  int G=0;
  int B=0;

  // load an image
  img[0]= cvLoadImage(img_name,-1);

  if(!img)
  {
    printf("Could not load image file: %s\n",img_name);
    exit(0);
  }

  // get the image data
  height    = img[0]->height;
  width     = img[0]->width;
  step      = img[0]->widthStep;
  channels  = img[0]->nChannels;

  data      = (uchar *)img[0]->imageData;
  printf("Processing a %dx%d image with %d channels\n",height, width, channels);
  printf( "Image size: %d bytes\n", img[0]->imageSize );
  printf( "Width step: %d bytes\n", img[0]->widthStep );

    //initialisation de l'histo:
for(i=0;i<48;i++)
{
    histo_16xRGB[i]=0;
}

 // printf("%d %d %d\t",(int)pixel.val[0],(int)pixel.val[1],(int)pixel.val[2]); //
 for (i=0;i<height;i++)
 {
     for(j=0;j<width;j++)
     {
         pixel = cvGet2D(img[0],i,j); // ordre du résultat de la commande: B G R
         B=(int)(((int)pixel.val[0])/16);
         G=(int)(((int)pixel.val[1])/16);
         R=(int)(((int)pixel.val[2])/16);

         if(B<256 && G <256 && R<256) // vérification, pour éviter tout pb
         {
             histo_16xRGB[B+32]++;
             histo_16xRGB[G+16]++;
             histo_16xRGB[R]++;
         }
         else
         {
             printf("erreur valeur composante");
             exit(0);
         }
     }
 }

printf("\nR: ");
for(i=0;i<16;i++)
    printf(" %d",histo_16xRGB[i]);

printf("\nG: ");
for(i=0;i<16;i++)
    printf(" %d",histo_16xRGB[i+16]);

printf("\nB: ");
for(i=0;i<16;i++)
    printf(" %d",histo_16xRGB[i+32]);

    // create a window
  cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
  cvMoveWindow("mainWin", 100, 100);

    // show the image
  cvShowImage("mainWin", img[0] );

  // wait for a key
  cvWaitKey(0);

  // release the image
  cvReleaseImage(&img[0] );
}


void compute_histo_256(char * img_name)
{
  // variables
  IplImage* img[1];
  int height,width,step,channels;
  uchar *data;
  int i,j,k;

  // load an image
  img[0]= cvLoadImage(img_name,-1);

  if(!img)
  {
    printf("Could not load image file: %s\n",img_name);
    exit(0);
  }

  // get the image data
  height    = img[0]->height;
  width     = img[0]->width;
  step      = img[0]->widthStep;
  channels  = img[0]->nChannels;

  data      = (uchar *)img[0]->imageData;
  printf("Processing a %dx%d image with %d channels\n",height, width, channels);
  printf( "Image size: %d bytes\n", img[0]->imageSize );
  printf( "Width step: %d bytes\n", img[0]->widthStep );


 // printf("%d %d %d\t",(int)pixel.val[0],(int)pixel.val[1],(int)pixel.val[2]); //
  int histoR[256], histoG[256], histoB[256];
  CvScalar pixel;

  for(i=0; i<=255 ; i++)
  {
          histoR[i] = 0;
          histoG[i] = 0;
          histoB[i] = 0;
  }


  for(i=0; i<height ; i++)
  {
      for(j=0 ; j< width; j++)
      {
          pixel = cvGet2D (img[0],i,j);
          histoR[(int)pixel.val[2]] ++;
          histoG[(int)pixel.val[1]] ++;
          histoB[(int)pixel.val[0]] ++;
      }
  }

printf("\nR: ");
for(i=0;i<16;i++)
    printf("niveau de rouge %d = %d\n",i,histoR[i]);

printf("\nG: ");
for(i=0;i<16;i++)
    printf("niveau de vert %d = %d\n",i,histoR[i]);

printf("\nB: ");
for(i=0;i<16;i++)
    printf("niveau de bleue %d = %d\n",i,histoR[i]);
    printf("\n");


    // create a window
  cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
  cvMoveWindow("mainWin", 100, 100);

    // show the image
  cvShowImage("mainWin", img[0] );

  // wait for a key
  cvWaitKey(0);

  // release the image
  cvReleaseImage(&img[0]);
}

int intersection_histo(int * histo1, int * histo2, int histoSize)
{
    int i=0;
    float num= 0, den =0, dist_col=0;

  for(i=0 ; i<histoSize ; i++)
  {
      num = num + abs(histo1[i]-histo2[i]);
      den = den + (histo1[i]+histo2[i]);
  }

  dist_col = num/den;
  printf("intersection = %f",dist_col);
  printf("\n");

    return dist_col;
}


void greylevel(IplImage* img[0])
{

  IplImage *image_nvg = cvCreateImage(cvGetSize(img[0]), img[0]->depth, 1);
  cvConvertImage(img[0], image_nvg,NULL);
  img[0]=image_nvg;

  // release the image
  //cvReleaseImage(&image_nvg );

/*

  // get the image data
  height    = img[0]->height;
  width     = img[0]->width;
  step      = img[0]->widthStep;
  channels  = img[0]->nChannels;

  data      = (uchar *)img[0]->imageData;
  printf("Processing a %dx%d image with %d channels\n",height,width,channels);
  printf( "Image size: %d bytes\n", img[0]->imageSize );
  printf( "Width step: %d bytes\n", img[0]->widthStep );

  CvScalar pixel, s;
  int i,j;
  float value=0;

  for(i=0; i<height ; i++)
  {
      for(j=0 ; j< width; j++)
      {
          pixel = cvGet2D (img[0],i,j);
          value = pixel.val[2]*0.299 + pixel.val[1]*0.587 + pixel.val[0]*0114;
          s.val[0]= (int)value;
          s.val[1]= (int)value;
          s.val[2]= (int)value;
          cvSet2D (img[0],i,j,s);
      }
  }
  */

}

void reduce_greylevel(IplImage* img[0], int level)
{
    CvScalar pixel;
    int i,j,height,width,value,res;
    height    = img[0]->height;
    width     = img[0]->width;

    //printf("%d\t%d\n",height,width);

  for(i=0; i<height ; i++)
  {
      for(j=0 ; j< width; j++)
      {
          pixel = cvGet2D (img[0],i,j);
          value = (int) ((pixel.val[0] / 256) * level);
          //res = value*256 / level ;
          pixel.val[0] = value;
          cvSet2D (img[0],i,j,pixel);


      }
  }
}


