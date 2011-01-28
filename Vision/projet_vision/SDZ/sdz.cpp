#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <iostream>

//#include <cv.h>
//#include <highgui.h>

using namespace std;

IplImage * img;
IplImage * img_bin;
int seuil;

void invert_grayscale(IplImage *img)
{
	
	IplImage *img_nvg=cvCreateImage(cvGetSize(img), img->depth, 1); // cvCloneImage(img);
	cvConvertImage(img, img_nvg,0);// convertir niveaux de gris
	int ligne,colone;
	CvScalar pixel;

	for (ligne=0 ; ligne < img_nvg->height ; ligne++)
	{
		for (colone=0 ; colone < img_nvg->width ; colone++)
		{
			pixel = cvGet2D(img_nvg,ligne,colone);
			pixel.val[0] = 255 - pixel.val[0];
			cvSet2D(img_nvg,ligne,colone,pixel);
		}
	}
//   cout << "pixel(10,10)=" << pixel.val[0] << endl;

	cvNamedWindow("lena", CV_WINDOW_AUTOSIZE);
	cvShowImage("lena", img_nvg);
	cvWaitKey(0);
	cvDestroyWindow("lena");
	cvReleaseImage(&img_nvg);
}

void stretch_histo(IplImage *img)
{
	int p_min=255, p_max = 0;
	int ligne,colone;
	CvScalar pixel;

	for (ligne=0 ; ligne < img->height ; ligne++)
	{
		for (colone=0 ; colone < img->width ; colone++)
		{
			pixel = cvGet2D(img,ligne,colone);
			if (pixel.val[0]<p_min) p_min = pixel.val[0];
			if (pixel.val[0]>p_max) p_max = pixel.val[0];
		}
	}

	for (ligne=0 ; ligne < img->height ; ligne++)
	{
		for (colone=0 ; colone < img->width ; colone++)
		{
			pixel = cvGet2D(img,ligne,colone);
			pixel.val[0] = (255*(pixel.val[0]-p_min))/(p_max-p_min);
			cvSet2D(img,ligne,colone,pixel);
		}
	}
	cvNamedWindow("stretch", CV_WINDOW_AUTOSIZE);
	cvShowImage("stretch", img);
	cvWaitKey(0);
	cvDestroyWindow("stretch");
}

void filtre_moyen(IplImage * img)
{
	if(img->nChannels!=1) return;  //On vérifie que l'image que l'on nous a fournie est bien en NVG
	IplImage * img_temp = cvCloneImage(img);
	int moyenne;
	int ligne,colone;
	CvScalar pixel;

	for (ligne=0 ; ligne < img->height ; ligne++)
	{
		for (colone=0 ; colone < img->width ; colone++)
		{
			 //S'il est au bord
            		if((colone < 2) || (colone > (img->width - 3)) || (ligne < 2) || (ligne > (img->height-3)))
            		{
		                cvSet2D(img_temp, ligne, colone, cvGet2D(img, ligne, colone));
			}
			else
			{
        		        moyenne=0;
        		        for( int i=ligne-2; i<=ligne+2; i++)
        		        {
        		            for(int j=colone-2; j<=colone+2; j++)
        		            {
        		                pixel = cvGet2D(img, i, j);
        		                moyenne+= pixel.val[0];                    
        		            }
        		        }
        		        pixel.val[0] = moyenne/25;
        		        cvSet2D(img_temp, ligne, colone, pixel);
        		 }
		}
	}

	img = cvCloneImage(img_temp);
	cvReleaseImage(&img_temp);
	cvNamedWindow("filtre_moyen", CV_WINDOW_AUTOSIZE);
	cvShowImage("filtre_moyen", img);
	cvWaitKey(0);
	cvDestroyWindow("filtre_moyen");
}

void filtre_median(IplImage * img)
{
	if(img->nChannels!=1) return;
	IplImage * img_temp = cvCloneImage(img);
	int moyenne;
	int ligne,colone;
	CvScalar pixel;
	int *voisins=new int[25];

	for (ligne=0 ; ligne < img->height ; ligne++)
	{
		for (colone=0 ; colone < img->width ; colone++)
		{
            		if((colone < 2) || (colone > (img->width - 3)) || (ligne < 2) || (ligne > (img->height-3)))
            		{
		                cvSet2D(img_temp, ligne, colone, cvGet2D(img, ligne, colone));
			}
			else
			{
        		        for( int i=ligne-2; i<=ligne+2; i++)
        		        {
        		            for(int j=colone-2; j<=colone+2; j++)
        		            {
        		                pixel = cvGet2D(img, i, j);
        		                voisins[(i-ligne+2)*5 + (j-colone+2)]=pixel.val[0];
        		            }
        		        }
				sort( voisins, voisins + 25); //On classe les valeurs
        		        pixel.val[0] = voisins[12];
        		        cvSet2D(img_temp, ligne, colone, pixel);
        		 }
		}
	}

	img = cvCloneImage(img_temp);
	cvReleaseImage(&img_temp);
	cvNamedWindow("filtre_median", CV_WINDOW_AUTOSIZE);
	cvShowImage("filtre_median", img);
	cvWaitKey(0);
	cvDestroyWindow("filtre_median");
}

void seuillage(int valeur)
{
	cvThreshold(img, img_bin, seuil, 255, CV_THRESH_BINARY);
	cvShowImage("binary", img_bin);
}

void binary_image()
{
	img_bin = cvCloneImage(img);
	seuil = 127;

	cvNamedWindow("binary", CV_WINDOW_AUTOSIZE);	
	cvThreshold(img, img_bin, seuil, 255, CV_THRESH_BINARY);
	cvShowImage("binary", img_bin);

	cvCreateTrackbar( "Seuil de binarisation : ", "binary", &seuil , 255 , seuillage );

	cvWaitKey(0);
	cvDestroyWindow("binary");
}


int main()
{

   //IplImage *img=cvLoadImage("lena.jpg",CV_LOAD_IMAGE_COLOR);
   //IplImage *img=cvLoadImage("114169.png",CV_LOAD_IMAGE_GRAYSCALE);
   img=cvLoadImage("lena.jpg",CV_LOAD_IMAGE_GRAYSCALE);

   //invert_grayscale(img);
   //stretch_histo(img);
   //filtre_median(img);
   binary_image(); //activer les valeurs globales

   //cvReleaseImage(&img);

   return 0;
}
