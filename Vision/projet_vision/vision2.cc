#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>




////////////////////////////////////////////////////////////


int main()
{
    // Image qui contiendra les frames de la vidéo
    IplImage* img;
    IplImage* img2;
  
    // La capture vidéo, ici, un fichier :
    CvCapture* capture = cvCreateFileCapture("/home/olivier/Vidéos/capture4/webcam1.avi");
    CvCapture* capture2 = cvCreateFileCapture("/home/olivier/Vidéos/capture4/webcam2.avi");

    // Si vous voulez lire une webcam à la place :
    //CvCapture* capture = cvCreateCameraCapture( 1 );
    //CvCapture* capture2 = cvCreateCameraCapture( -1 );
  
    // On vérifie que la capture est bien initialisée,
    // si ce n'est pas le cas, on quitte le programme.
    if( !capture ) 
        return 10;
    if( !capture2 ) 
        return 11;

   // cvNamedWindow("video", CV_WINDOW_AUTOSIZE);
    //cvNamedWindow("video2", CV_WINDOW_AUTOSIZE);
    char key;

    // On essaye de capturer une frame
    if(!cvGrabFrame( capture ))
        return 20;
    if(!cvGrabFrame( capture2 ))
        return 21;
  
    // Tant qu'on n'a pas appuyé sur Q, on continue :
    while(key != 'q')
    {
        // On décompresse la dernière frame récupérée...
        img = cvRetrieveFrame( capture );
	img2 = cvRetrieveFrame( capture2 );

/////////////////////// FILTRE LAPLACIEN ///////////////////////////

	IplImage* img_nvg   = cvCreateImage( cvGetSize( img ), IPL_DEPTH_8U, 1);
	IplImage* img_nvg2   = cvCreateImage( cvGetSize( img2 ), IPL_DEPTH_8U, 1);
	IplImage* laplace = cvCloneImage( img_nvg );
	IplImage* laplace2 = cvCloneImage( img_nvg2 );

	int flip = 0;
	if( img->origin != IPL_ORIGIN_TL ) flip = CV_CVTIMG_FLIP;
	cvConvertImage(img, img_nvg, flip);
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

	flip = 0;
	if( img2->origin != IPL_ORIGIN_TL ) flip = CV_CVTIMG_FLIP;
	cvConvertImage(img2, img_nvg2, flip);
	cvFilter2D( img_nvg2, laplace2, laplacien );

///////////////////////////////////////////////////////////////////////

	


       // cvShowImage( "video", img);
	//cvShowImage( "video2", img2 );

        // On attend 10 ms : IMPORTANT !
        key = cvWaitKey(10);
    
        // On essaye de capturer la frame suivante
        if(!cvGrabFrame(capture) || !cvGrabFrame(capture2))
            key = 'q'; 

	cvReleaseMat(&laplacien);
	cvReleaseImage( &img_nvg );
	cvReleaseImage( &img_nvg2 );
	cvReleaseImage( &laplace );
	cvReleaseImage( &laplace2 );

    }

    // On détruit les fenêtres créées : nouvelle fonction. ;)  
    cvDestroyAllWindows();

    // On n'a pas besoin de détruire l'image.
    img = NULL;
    img2 = NULL;

    // On détruit la capture
    cvReleaseCapture( &capture );
    cvReleaseCapture( &capture2 );

    return 0; 
}

