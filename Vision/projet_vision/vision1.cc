#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define COMPUTATION_WINDOW_SIZE 4
#define ABSOLUTE_MAX_CORNERS 100
#define inImage1 0
#define inImage2 1


CvPoint2D32f** matchedCorners;
IplImage * img1;
IplImage * img2;
IplImage * img1Col;
IplImage * img2Col;
IplImage * eigImage; // image temporaire
IplImage * eigImagebis; // image temporaire
IplImage * tempImage ; // image temporaire
IplImage * tempImagebis;
IplImage * mergedImage;
CvPoint2D32f* corners; // tableau des coins
CvPoint2D32f* cornersbis;
int cornerCount;
int cornerCountbis;
int * ptRealCornerCount;


int maxNumberOfCorners;
int blockSize;
double minDistance;
double qualityDetectionLevel;



void displayCorners(CvPoint2D32f* corners1, int count1, CvPoint2D32f* corners2, int count2, IplImage* image1, IplImage* image2)
{
    int i;
    CvPoint pointCenter;

    for (i=0; i < count1; i++)
    {
        pointCenter = cvPointFrom32f(corners1[i]);
        cvCircle(image1, pointCenter, 5, CV_RGB(0, 0, 255),2,8,0);
    }
    for (i=0; i < count2; i++)
    {
        pointCenter = cvPointFrom32f(corners2[i]);
        cvCircle(image2, pointCenter, 5, CV_RGB(255, 255, 255),2,8,0);
    }

    cvShowImage("Image1", image1 );
    cvShowImage("Image2", image2 );
}



void drawLinesBetweenSimilarCorners(int cornersCount, CvPoint2D32f** similarCornersList, IplImage * doubleImage, IplImage* image1, IplImage* image2)
{
    int i,j;
    CvScalar pix;

    // parse and copy images into the new one
    for(i=0;i<image1->height;i++){
        for(j=0;j<image1->width;j++){
            pix = cvGet2D(image1,i,j);
            cvSet2D(doubleImage,i,j,pix);
        }
    }
    for(i=0;i<image2->height;i++){  // starts with j unchanged (=last line of image1)
        for(j=0;j<image1->width;j++){
            pix = cvGet2D(image2,i,j);
            cvSet2D(doubleImage,i +image1->height,j,pix);
        }
    }

    // an offset must be added to the second image corners coordinates
    for(i=0;i<cornersCount;i++){
        cvLine(doubleImage,
               cvPoint(similarCornersList[inImage1][i].x, similarCornersList[inImage1][i].y),
               cvPoint(similarCornersList[inImage2][i].x, similarCornersList[inImage2][i].y + image1->height),
               CV_RGB( 100, 20, 100), 1, CV_AA, 0);
    }
     cvShowImage("Corresponding Corners", doubleImage);
}



float ZSSD(int K, CvPoint2D32f point1, CvPoint2D32f point2, IplImage* image1, IplImage* image2)
{
    int i,j;
    int result =0;
    int x1 = (int)point1.x;
    int y1 = (int)point1.y;
    int x2 = (int)point2.x;
    int y2 = (int)point2.y;
    int I1 = 0;
    int I2 = 0;

    // Get average value for the current window
    for(i=-K/2;i<=K/2;i++){
        for(j=-K/2;j<=K/2;j++){
                printf("%d\t",I1);
            I1 += cvGet2D(image1,y1+i,x1+j).val[0];
            I2 += cvGet2D(image2,y2+i,x2+j).val[0];
        }
    }
    I1 = I1/(i * j); //i*j = nomber of pixels in the window
    I2 = I2/(i * j);
    printf("%d\n",I1);

    // if K is even => window size =K+1
    for(i=-K/2;i<=K/2;i++){
        for(j=-K/2;j<=K/2;j++){
            //printf("%d\n",i);
            result += pow( (cvGet2D(image1,y1+i,x1+j).val[0] - I1)
                          - (cvGet2D(image2,y2+i,x2+j).val[0] - I2)
                          ,2 );
        }
    }

    return -result;
}

float ZNCC(int K, CvPoint2D32f point1, CvPoint2D32f point2, IplImage* image1, IplImage* image2)
{
    int i,j;
    double num= 0, den1= 0, den2= 0;
    int pixelCount1=0, pixelCount2=0;
    int x1 = (int)point1.x;
    int y1 = (int)point1.y;
    int x2 = (int)point2.x;
    int y2 = (int)point2.y;
    int I1 = 0;
    int I2 = 0;

    // Get average value for the current window
    for(i=-K/2;i<=K/2;i++){
        for(j=-K/2;j<=K/2;j++){
            if((y1+i)>0 && (x1+j)>0 && (x1+j)<image1->width && (y1+i)<image1->height){
               I1 += cvGet2D(image1,y1+i,x1+j).val[0];
               pixelCount1++;
            }
            if((y2+i)>0 && (x2+j)>0 && (x2+j)<image2->width && (y2+i)<image2->height){
                I2 += cvGet2D(image2,y2+i,x2+j).val[0];
                pixelCount2++;
            }
        }
    }
    I1 = I1/pixelCount1;
    I2 = I2/pixelCount2;

    // if K is even => window size =K+1
    for(i=-K/2;i<=K/2;i++){
        for(j=-K/2;j<=K/2;j++){
            if((y1+i)>0 && (x1+j)>0 && (x1+j)<image1->width && (y1+i)<image1->height &&
                (y2+i)>0 && (x2+j)>0 && (x2+j)<image2->width && (y2+i)<image2->height){
                num += (cvGet2D(image1,y1+i,x1+j).val[0] - I1)*(cvGet2D(image2,y2+i,x2+j).val[0] - I2);
                den1 += pow(cvGet2D(image1,y1+i,x1+j).val[0] -I1 ,2 );
                den2 += pow(cvGet2D(image2,y2+i,x2+j).val[0] -I2 ,2 );
            }
        }
    }

    return num/(sqrt(den1)*sqrt(den2));
}

// The result is a bijection
CvPoint2D32f** matchCorners(CvPoint2D32f*corn1, int corn1Count, CvPoint2D32f*corn2, int corn2Count,int * ptRealMatchedCount,IplImage *img1Copy, IplImage *img2Copy)
{
    CvPoint2D32f** reallyMatchedCorners =NULL;
    *ptRealMatchedCount=0;
    int  i, j ,tempIndexMax;
    int indexTable[ABSOLUTE_MAX_CORNERS]; // store all indexes found for image1 (this array is based on corn1)
    int goodMatches[ABSOLUTE_MAX_CORNERS][2]; // (a lot) smaller than the previous one
    float temp, max;

    // 1 => 2
    for(i=0;i<corn1Count;i++){
        max=INT_MIN;
        for(j=0;j<corn2Count;j++){
            temp= ZNCC(COMPUTATION_WINDOW_SIZE,corn1[i],corn2[j],img1Copy,img2Copy);
            //printf("%f\n",temp);
            if(temp>max){
                max= temp;
                tempIndexMax = j;
            }
        }
        indexTable[i]=tempIndexMax; // indexation is based on the first cornersList from image1
    }
    // 2 => 1
    for(i=0;i<corn2Count;i++){
        max=INT_MIN;
        for(j=0;j<corn1Count;j++){
            temp= ZNCC(COMPUTATION_WINDOW_SIZE,corn2[i],corn1[j],img2Copy,img1Copy);
            //printf("%f\n",temp);
            if(temp>max){
                max= temp;
                tempIndexMax= j;
            }
        }
        // time to check if the similarity is bijective
        if(indexTable[tempIndexMax] == i){
            (*ptRealMatchedCount)++;
            goodMatches[(*ptRealMatchedCount)-1][inImage1]=tempIndexMax; // index in corn1
            goodMatches[(*ptRealMatchedCount)-1][inImage2]=i; // corresponding index in corn2
        }

    }

    // result to return
    reallyMatchedCorners = (CvPoint2D32f**) malloc(2 * sizeof(CvPoint2D32f *)); // Store final result
    reallyMatchedCorners[inImage1]= (CvPoint2D32f*) malloc(*ptRealMatchedCount * sizeof(CvPoint2D32f));
    reallyMatchedCorners[inImage2]= (CvPoint2D32f*) malloc(*ptRealMatchedCount * sizeof(CvPoint2D32f));

    for(i=0;i<(*ptRealMatchedCount);i++){
        j= goodMatches[i][inImage1];
        reallyMatchedCorners[inImage1][i]=corn1[j];
        j= goodMatches[i][inImage2];
        reallyMatchedCorners[inImage2][i]=corn2[j];
    }

    printf("real matched points: %d\n",*ptRealMatchedCount);
    return reallyMatchedCorners;
}


void refreshCorners()
{
   // cvDestroyWindow("Corresponding Corners");
    IplImage * img1Copy = cvCloneImage(img1);
    IplImage * img2Copy = cvCloneImage(img2);
    IplImage * img1CopyCol = cvCloneImage(img1Col);
    IplImage * img2CopyCol = cvCloneImage(img2Col);
    IplImage * mergedImageCopy = cvCloneImage(mergedImage);

    // Initial values of trackbars
    maxNumberOfCorners= 50; // 10
    qualityDetectionLevel= 0.8; // 0.7
    blockSize=30; // 3
    minDistance=4.0; // 4.0

    //if(corners != NULL) free(corners);
    //if(cornersbis != NULL) free(cornersbis);
    cornerCount = maxNumberOfCorners;
    cornerCountbis = maxNumberOfCorners;

    // Find Corners
    cvGoodFeaturesToTrack(
        img1Copy,eigImage,tempImage,
        corners,&cornerCount,
        qualityDetectionLevel, // Quality Level
        minDistance, // Minimum Distance between points
        0,
        blockSize,
        0,  // Use of Harris
        0.4);

     cvGoodFeaturesToTrack(
        img2Copy,eigImagebis,tempImagebis,
        cornersbis,&cornerCountbis,
        qualityDetectionLevel,
        minDistance,
        0,
        blockSize,
        0,  // Use of Harris
        0.4);

    // check
    printf("\nNumber of corners found:\nimg1:%d\timg2:%d\n",cornerCount,cornerCountbis);

    if(cornerCount != cornerCountbis){
        printf("(Not the same number of corners found in both images)\n");}

    displayCorners(corners,cornerCount,cornersbis,cornerCountbis,img1Copy,img2Copy);

    // Core Algorithm: Match similar corners in both lists
    matchedCorners = matchCorners(corners,cornerCount,cornersbis,cornerCountbis,ptRealCornerCount,img1Copy,img2Copy);

    if(img1Col != NULL && img2Col != NULL)
        drawLinesBetweenSimilarCorners(*ptRealCornerCount, matchedCorners, mergedImageCopy, img1CopyCol, img2CopyCol);
    else
        drawLinesBetweenSimilarCorners(*ptRealCornerCount, matchedCorners, mergedImageCopy, img1Copy, img2Copy);

    cvReleaseImage(&img1Copy);
    cvReleaseImage(&img2Copy);
    cvReleaseImage(&img1CopyCol);
    cvReleaseImage(&img2CopyCol);
    cvReleaseImage(&mergedImageCopy);
}


CvPoint2D32f** findSimilarCorners(IplImage * image1Gray, IplImage* image2Gray, int * nbCorners, IplImage * image1Color, IplImage* image2Color)
{


    // Warning between upper and lower case !
    //printf("cvPoint: %d \t CvPoint: %d\n\n",sizeof(cvPoint2D32f),sizeof(CvPoint2D32f));
    matchedCorners= NULL;
    ptRealCornerCount = nbCorners;
    img1 = cvCloneImage(image1Gray);
    img2 = cvCloneImage(image2Gray);
    img1Col = cvCloneImage(image1Color);
    img2Col = cvCloneImage(image2Color);
    eigImage = cvCreateImage(cvGetSize(img1),IPL_DEPTH_32F,1); // image temporaire
    eigImagebis = cvCreateImage(cvGetSize(img2),IPL_DEPTH_32F,1); // image temporaire
    tempImage = cvCreateImage(cvGetSize(eigImage),IPL_DEPTH_32F,1); // image temporaire
    tempImagebis = cvCreateImage(cvGetSize(eigImagebis),IPL_DEPTH_32F,1); // image temporaire

    corners = (CvPoint2D32f*) malloc(100 * sizeof(CvPoint2D32f));
    cornersbis = (CvPoint2D32f*) malloc(100 * sizeof(CvPoint2D32f));

    // build the double image
    if(img1Col != NULL && img2Col != NULL)
        mergedImage = cvCreateImage(cvSize(cvGetSize(img1).width, cvGetSize(img1).height * 2), img1Col->depth,img1Col->nChannels);
    else
        mergedImage = cvCreateImage(cvSize(cvGetSize(img1).width, cvGetSize(img1).height * 2), img1->depth,img1->nChannels);


    refreshCorners();

    cvNamedWindow("Image1", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Image2", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Corresponding Corners", CV_WINDOW_AUTOSIZE);
   
    cvShowImage("Image1", img1);
    cvShowImage("Image2", img2);
   // cvWaitKey(0);


    // End
    free(corners);
    free(cornersbis);
    cvReleaseImage(&img1);
    cvReleaseImage(&img2);
    cvReleaseImage(&img1Col);
    cvReleaseImage(&img2Col);
    cvReleaseImage(&eigImage);
    cvReleaseImage(&tempImage);
    cvReleaseImage(&eigImagebis);
    cvReleaseImage(&tempImagebis);
    cvReleaseImage(&mergedImage);
    return matchedCorners;
}




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

	int numberOfCorners=0;

	IplImage * imageGray = cvCloneImage(laplace); //img_nvg
	IplImage * imageColor = cvCloneImage(laplace); //img
	IplImage * imagebisGray = cvCloneImage(laplace2); //img_nvg2
	IplImage * imagebisColor = cvCloneImage(laplace2); //img2

	//CvPoint2D32f** correspondingCornersCoordinates= findSimilarCorners(imageGray, imagebisGray, &numberOfCorners, imageColor, imagebisColor);




/*
IplImage * image = cvCloneImage(img_nvg);
cvCvtColor(img,image,CV_BGR2GRAY);
cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
cvShowImage("image",image);
*/



cvNamedWindow("video", CV_WINDOW_AUTOSIZE);


        cvShowImage( "video", laplace);
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

