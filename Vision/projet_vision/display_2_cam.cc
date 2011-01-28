#include <opencv/highgui.h>
#include <opencv/cv.h>

int main()
{
    // Image qui contiendra les frames de la vidéo
    IplImage* img;
    IplImage* img2;
  
    // La capture vidéo, ici, un fichier :
    //CvCapture* capture = cvCreateFileCapture("/home/olivier/Vidéos/capture4/webcam1.avi");
    //CvCapture* capture2 = cvCreateFileCapture("/home/olivier/Vidéos/capture4/webcam2.avi");

    // Si vous voulez lire une webcam à la place :
    CvCapture* capture = cvCreateCameraCapture( 1 );
    CvCapture* capture2 = cvCreateCameraCapture( -1 );
  
    // On vérifie que la capture est bien initialisée,
    // si ce n'est pas le cas, on quitte le programme.
    if( !capture ) 
        return 10;
    if( !capture2 ) 
        return 11;

    cvNamedWindow("video", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("video2", CV_WINDOW_AUTOSIZE);
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
    
        // ... et on la montre à l'écran.
        cvShowImage( "video", img );
	cvShowImage( "video2", img2 );

        // On attend 10 ms : IMPORTANT !
        key = cvWaitKey(10);
    
        // On essaye de capturer la frame suivante
        if(!cvGrabFrame( capture ))
            key = 'q';  

	if(!cvGrabFrame( capture2 ))
            key = 'q';  
/*
cvWaitKey();
cvSaveImage("left.jpg", img);
cvSaveImage("right.jpg", img2);
*/


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

