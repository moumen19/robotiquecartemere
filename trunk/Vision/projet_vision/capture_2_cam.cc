#include <opencv/highgui.h>
#include <opencv/cv.h>

int main()
{
    IplImage* img;
    IplImage* img2;
  
    // La capture vidéo
    CvCapture* capture = cvCreateCameraCapture( 1 );
    CvCapture* capture2 = cvCreateCameraCapture( -1 );

    if( !capture ) 
        return 10;

    if( !capture2 ) 
        return 10;

    cvNamedWindow("video1", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("video2", CV_WINDOW_AUTOSIZE);
    char key;
    
    img=cvQueryFrame( capture );
    img2=cvQueryFrame( capture2 );

    if( !img )
        return 20;

    // On initialise le writer
    // Ne connaissant pas le frame rate de la webcam, on suppose que c'est 10 FPS
    CvVideoWriter* writer = cvCreateVideoWriter( 
                                "/home/olivier/Vidéos/webcam1.avi",  // chemin du fichier
                                CV_FOURCC('X','V','I','D'),        // codec
                                20. ,                              // frame-rate
                                cvGetSize(img)                     // définition
                            );
  
    CvVideoWriter* writer2 = cvCreateVideoWriter( 
                                "/home/olivier/Vidéos/webcam2.avi",  // chemin du fichier
                                CV_FOURCC('X','V','I','D'),        // codec
                                20. ,                              // frame-rate
                                cvGetSize(img2)                     // définition
                            );

    while(key != 'q')
    {
        // On montre l'image à l'écran
        cvShowImage( "video1", img );
	cvShowImage( "video2", img2 );
                
        // On écrit la frame dans le fichier vidéo
        cvWriteFrame( writer, img);
        cvWriteFrame( writer2, img2);

        // On capture la frame suivante
        img = cvQueryFrame( capture );
	img2 = cvQueryFrame( capture2 );

        // On attend 10 ms
        key = cvWaitKey(10);
    }

    cvDestroyAllWindows();
    img = NULL;
    cvReleaseCapture( &capture );
    img2 = NULL;
    cvReleaseCapture( &capture2 );

    // On détruit le writer
    cvReleaseVideoWriter( &writer );
    cvReleaseVideoWriter( &writer2 );

    return 0; 
}

