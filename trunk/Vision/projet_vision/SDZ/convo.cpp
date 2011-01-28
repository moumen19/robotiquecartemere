#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <iostream>


using namespace std;


void convo_gauss(IplImage * img)
{

	IplImage* img_nvg   = cvCreateImage( cvGetSize( img ), IPL_DEPTH_8U, 1);
	int flip = 0;
	if( img->origin != IPL_ORIGIN_TL ) flip = CV_CVTIMG_FLIP;
	cvConvertImage(img, img_nvg, flip);

	// Résultats des filtres
	IplImage* lissee    = cvCloneImage( img_nvg ); 
	
	// Masques de convolution 3x3 
	CvMat* gaussien = cvCreateMat( 3, 3, CV_32FC1);

	// Remplissage du masque du filtre gaussien « approximatif » :

	// |   1   2   1   |
	// |   2   4   2   |
	// |   1   2   1   |
	// somme = 16

	cvSet( gaussien, cvScalar( 1.0 / 16.0 ) );
	cvmSet( gaussien, 0, 1, 2.0 / 16.0 );
	cvmSet( gaussien, 1, 0, 2.0 / 16.0 );
	cvmSet( gaussien, 1, 2, 2.0 / 16.0 );
	cvmSet( gaussien, 2, 1, 2.0 / 16.0 );
	cvmSet( gaussien, 1, 1, 4.0 / 16.0 );

	cvFilter2D( img_nvg, lissee, gaussien );

	cvNamedWindow( "originale", CV_WINDOW_AUTOSIZE );
	cvNamedWindow("gauss", CV_WINDOW_AUTOSIZE);
	cvShowImage( "originale", img_nvg );
	cvShowImage("gauss", lissee);
	cvWaitKey(0);
	cvDestroyAllWindows();

	cvReleaseImage(&img_nvg);
	cvReleaseImage(&lissee);
	cvReleaseMat(&gaussien);

}

void convo_rehauss(IplImage * img)
{

	IplImage* img_nvg   = cvCreateImage( cvGetSize( img ), IPL_DEPTH_8U, 1);
	int flip = 0;
	if( img->origin != IPL_ORIGIN_TL ) flip = CV_CVTIMG_FLIP;
	cvConvertImage(img, img_nvg, flip);

	IplImage* rehaussee = cvCloneImage( img_nvg );

	CvMat* rehaussement = cvCreateMat( 3, 3, CV_32FC1);

	// Remplissage du masque du rehaussement de contours :

	// |   0   -1    0   |
	// |  -1    5   -1   |
	// |   0   -1    0   |

	cvSet( rehaussement, cvScalar( 0 ) );
	cvmSet( rehaussement, 0, 1, -1 ); 
	cvmSet( rehaussement, 1, 0, -1 );
	cvmSet( rehaussement, 1, 2, -1 );
	cvmSet( rehaussement, 2, 1, -1 );
	cvmSet( rehaussement, 1, 1,  5 );

	cvFilter2D( img_nvg, rehaussee, rehaussement );

	cvNamedWindow( "originale", CV_WINDOW_AUTOSIZE );
	cvNamedWindow("rehaussee", CV_WINDOW_AUTOSIZE);
	cvShowImage( "originale", img_nvg );
	cvShowImage("rehaussee", rehaussee);
	cvWaitKey(0);
	cvDestroyAllWindows();

	cvReleaseImage(&img_nvg);
	cvReleaseImage(&rehaussee);
	cvReleaseMat(&rehaussement);
}

void normalisationNVG( IplImage *img )
{
    if( img->nChannels != 1 )
        return;
	
    CvScalar scalaire = cvGet2D( img, 0, 0);

    double p_min = scalaire.val[0];
    double p_max = scalaire.val[0];


    // Récupération de p_min et p_max
    for( int x = 0; x < img->width; x++ )
    {
        for( int y = 0; y < img->height; y++)
        {
            scalaire = cvGet2D( img, y, x);
            if( scalaire.val[0] < p_min )
                p_min = scalaire.val[0];

            if( scalaire.val[0] > p_max )
                p_max = scalaire.val[0];
        }
    }

    // Normalisation
    for( int x = 0; x < img->width; x++ )
    {
        for( int y = 0; y < img->height; y++)
        {
            scalaire = cvGet2D( img, y, x );

            scalaire.val[0] = 255 * ( scalaire.val[0] - p_min );
            scalaire.val[0] = cvRound( scalaire.val[0] / (p_max - p_min) );

            cvSet2D( img, y, x, scalaire );
        }
    }
}

void convo_laplace(IplImage * img)
{

	IplImage* img_nvg   = cvCreateImage( cvGetSize( img ), IPL_DEPTH_8U, 1);

	IplImage* laplace = cvCloneImage( img_nvg );
	IplImage* laplace_normalise = cvCloneImage( img_nvg );
	IplImage* laplace_normalise_16S = cvCreateImage( cvGetSize( img ), IPL_DEPTH_16S, 1); 
	IplImage* img_nvg_16S = cvCloneImage( laplace_normalise_16S );

	int flip = 0;
	if( img->origin != IPL_ORIGIN_TL ) flip = CV_CVTIMG_FLIP;
	cvConvertImage(img, img_nvg, flip);
	cvConvert(img_nvg, img_nvg_16S);

	CvMat* laplacien = cvCreateMat( 3, 3, CV_32FC1);

	// Remplissage du laplacien :

	// |   0   -1    0   |
	// |  -1    4   -1   |
	// |   0   -1    0   |

	cvSet( laplacien, cvScalar( 0 ) );
	cvmSet( laplacien, 0, 1, -1 ); 
	cvmSet( laplacien, 1, 0, -1 );
	cvmSet( laplacien, 1, 2, -1 );
	cvmSet( laplacien, 2, 1, -1 );
	cvmSet( laplacien, 1, 1,  4 );

	cvFilter2D( img_nvg, laplace, laplacien );
	cvFilter2D( img_nvg_16S, laplace_normalise_16S, laplacien );

	normalisationNVG( laplace_normalise_16S );
	cvConvert( laplace_normalise_16S, laplace_normalise );

	cvNamedWindow( "originale", CV_WINDOW_AUTOSIZE );
	cvNamedWindow("laplacien", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("laplacien_normalise", CV_WINDOW_AUTOSIZE);
	cvShowImage( "originale", img_nvg );
	cvShowImage("laplacien", laplace);
	cvShowImage("laplacien_normalise", laplace_normalise);
	cvWaitKey(0);
	cvDestroyAllWindows();

	cvReleaseImage( &img_nvg );
	cvReleaseImage( &img_nvg_16S);
	cvReleaseImage( &laplace );
	cvReleaseImage( &laplace_normalise );
	cvReleaseImage( &laplace_normalise_16S );

}

int main()
{

	IplImage* img = cvLoadImage( "lena.jpg");

	//convo_gauss(img);
	//convo_rehauss(img);
	convo_laplace(img);

	cvReleaseImage(&img);

   return 0;
}

