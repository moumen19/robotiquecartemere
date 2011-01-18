
#include "colorSegmentation.h"

void get_balls_pixelsSizes(int * ballsPixelsSizes, char * fileToAnalyse)
{
    int colorConfiguration;

    // Initialisation of global variables
    imgBGR = cvLoadImage(fileToAnalyse, -1);
    imgHSV = cvCloneImage(imgBGR);
    cvCvtColor(imgBGR, imgHSV, CV_BGR2HSV);
    grayMaskHSV = cvCreateImage(cvSize(imgHSV->width, imgHSV->height), imgHSV->depth, 1);
    improvedMaskOnBGR = cvCloneImage(imgBGR);
    maskInversion = 0;

    // For each ball
    for(colorConfiguration= red; colorConfiguration<= blue; colorConfiguration++)
    {
        /*// Values with good results ///*/
        if(colorConfiguration==red){
            Hmin=172;
            Hmax=3;
            Smin=113;
            Smax=237;
            Vmin = 0;
            Vmax=255;
        }
        if(colorConfiguration==green){
            Hmin=32;
            Hmax=71;
            Smin=66;
            Smax=242;
            Vmin = 0;
            Vmax=255;
        }
        if(colorConfiguration==blue){
            Hmin=108;
            Hmax=118;
            Smin=99;
            Smax=201;
            Vmin= 80;
            Vmax=255;
        }

        cvNamedWindow("mask", CV_WINDOW_AUTOSIZE);
        cvMoveWindow("mask", 1000, 20);
        cvSetMouseCallback("mask", colorMouseEventHandler, NULL); // mouse event defined for mask window
        cvNamedWindow("mask applied", CV_WINDOW_AUTOSIZE);
        cvMoveWindow("mask applied", 100, 100);

        cvCreateTrackbar("H min", "mask", &Hmin, 255, refresh_hue_min);
        cvCreateTrackbar("H max", "mask", &Hmax, 255, refresh_hue_max);
        cvCreateTrackbar("S min", "mask", &Smin, 255, refresh_saturation_min);
        cvCreateTrackbar("S max", "mask", &Smax, 255, refresh_saturation_max);
        cvCreateTrackbar("V min", "mask", &Vmin, 255, refresh_value_min);
        cvCreateTrackbar("V max", "mask", &Vmax, 255, refresh_value_max);
        cvCreateTrackbar("Inversion", "mask", &maskInversion, 1, refresh_inversion);

        segment_HSVimage();


        cvShowImage("mask", grayMaskHSV );
        cvShowImage("mask applied", improvedMaskOnBGR);
        cvWaitKey(200);

        // Getting size of remaining circle
        improve_and_apply_grayScaleMask(&grayMaskHSV,imgBGR,&improvedMaskOnBGR);
        ballsPixelsSizes[colorConfiguration] = pixelSize_of_binary_blob(grayMaskHSV);
    }

    // End
    cvDestroyAllWindows();
}

// non-destructive operation
void improve_and_apply_grayScaleMask(IplImage ** mask, IplImage * img, IplImage ** appliedMask)
{
    CvScalar pix = cvScalar(0,0,0,0);
    int i,j;

    ////////////////////////////////////////// improving gray level mask with Erosion & Dilatation //////////////////////
    IplImage * temp = cvCloneImage(*mask);
    IplImage * temp2 = cvCloneImage(temp);
    IplConvKernel* circleKernel = cvCreateStructuringElementEx(7,7,3,3,CV_SHAPE_ELLIPSE, NULL);

    cvMorphologyEx(mask,temp,temp2,circleKernel,CV_MOP_CLOSE,3);
    *mask = cvClone(temp);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    improvedMaskOnBGR = cvCloneImage(img);

    for(i=0;i<imgHSV->height;i++)
    {
        for(j=0;j<imgHSV->width;j++)
        {
            pix.val[0]= cvGet2D(*mask,i,j).val[0];
            if(pix.val[0]!= 0)
            {
                pix.val[0]= 0;
                pix.val[1]= 0;
                pix.val[2]= 0;
                cvSet2D(*appliedMask,i,j,pix);
            }

        }
    }

    cvReleaseImage(&temp);
    cvReleaseImage(&temp2);
    cvReleaseStructuringElement(&circleKernel);
}

// future: add a "shape" parameter, with a swith, to deal with all kinds of blobs
int pixelSize_of_binary_blob(IplImage * binaryImage)
{
    /////////////////////////////// Obvious points: N,W,E,S ///////////////////////
    int i,j;
    int a,b;
    int result= 0;
    int Xnorth=0,Ynorth=binaryImage->height;
    int Xwest=binaryImage->width,Ywest=0;
    int Xsouth=0,Ysouth=0;
    int Xeast=0,Yeast=0;
    int XnorthWest=0,YnorthWest=0;
    int XnorthEast=0,YnorthEast=0;
    int XsouthWest=0,YsouthWest=0;
    int XsouthEast=0,YsouthEast=0;

    for(i=0;i<binaryImage->height;i++)
    {
        for(j=0;j<binaryImage->width;j++)
        {

            if(cvGet2D(binaryImage,i,j).val[0] == 0)
            {
               // approximation by a square
               if(j>Xeast) {Xeast= j; Yeast= i;}
               if(i>Ysouth) {Ysouth= i; Xsouth= j;}
               if(j<Xwest) {Xwest= j; Ywest= i;}
               if(i<Ynorth) {Ynorth= i; Xnorth= j;}
            }

        }
    }
    result += (Ysouth-Ynorth) + (Xeast-Xwest);

    /////////////////////////////// Non-obvious points: NW,SW,SE,NE ///////////////////////
    // First method: determining X knowing Y
    YnorthWest= (Ywest+Ynorth)/2 ;
    YnorthEast= (Yeast+Ynorth)/2 ;
    YsouthWest= (Ysouth+Ywest)/2 ;
    YsouthEast= (Ysouth+Yeast)/2 ;

    // reach the circle
    i=Xwest;
    while(i<Xeast && XnorthWest==0){ if(cvGet2D(binaryImage,YnorthWest,i).val[0] == 0) XnorthWest= i; i++; }
    i=Xeast;
    while(i>Xwest && XnorthEast==0){ if(cvGet2D(binaryImage,YnorthEast,i).val[0] == 0) XnorthEast= i; i--; }
    i=Xwest;
    while(i<Xeast && XsouthWest==0){ if(cvGet2D(binaryImage,YsouthWest,i).val[0] == 0) XsouthWest= i; i++; }
    i=Xeast;
    while(i>Xwest && XsouthEast==0){ if(cvGet2D(binaryImage,YsouthEast,i).val[0] == 0) XsouthEast= i; i--; }

    // diagonal of a quadrilater: NW-SE
    a = YsouthEast-YnorthWest;
    b = XsouthEast-XnorthWest;
    result +=  sqrt(a*a + b*b);

    // diagonal of a quadrilater: NE-SW
    a = YsouthWest-YnorthEast;
    b = XnorthEast-XsouthWest;
    result +=  sqrt(a*a + b*b);

    // Second method: determining non-obvious points on the circle: computing Y knowing X
    XnorthWest= (Xwest+Xnorth)/2 ;  YnorthWest= 0;
    XnorthEast= (Xeast+Xnorth)/2 ;  YnorthEast= 0;
    XsouthWest= (Xsouth+Xwest)/2 ;  YsouthWest= 0;
    XsouthEast= (Xsouth+Xeast)/2 ;  YsouthEast= 0;

    // reach the circle
    i=Ynorth;
    while(i<Ysouth && YnorthWest==0){ if(cvGet2D(binaryImage,i,XnorthWest).val[0] == 0) YnorthWest= i; i++; }
    i=Ynorth;
    while(i<Ysouth && YnorthEast==0){ if(cvGet2D(binaryImage,i,XnorthEast).val[0] == 0) YnorthEast= i; i++; }
    i=Ysouth;
    while(i>Ynorth && YsouthEast==0){ if(cvGet2D(binaryImage,i,XsouthEast).val[0] == 0) YsouthEast= i; i--; }
    i=Ysouth;
    while(i>Ynorth && YsouthWest==0){ if(cvGet2D(binaryImage,i,XsouthWest).val[0] == 0) YsouthWest= i; i--; }

    // diagonal of a quadrilater: NW-SE
    a = YsouthEast-YnorthWest;
    b = XsouthEast-XnorthWest;
    result +=  sqrt(a*a + b*b);

    // diagonal of a quadrilater: NE-SW
    a = YsouthWest-YnorthEast;
    b = XnorthEast-XsouthWest;
    result +=  sqrt(a*a + b*b);

    return result/6; // average on 6 diameters
}

// To Determine color Thresholds
void segment_HSVimage(void)
{
    CvScalar pix = cvScalar(0,0,0,0);
    int i,j,detection;
    int H=0,S=0,V=0;

    for(i=0;i<imgHSV->height;i++)
    {
        for(j=0;j<imgHSV->width;j++)
        {
            H = cvGet2D(imgHSV,i,j).val[0];
            S = cvGet2D(imgHSV,i,j).val[1];
            V = cvGet2D(imgHSV,i,j).val[2];

            // Valide cases for a parameter:
            // 0 <min <X <max <255
            // 0 X <Hmax <Hmin <255
            // 0 <Hmax <Hmin <X <255

            /////////// Hue ///////////////////////////
            if(Hmin <=H && H <=Hmax)
                detection = 1;
            else if (Hmax <Hmin && (H <=Hmax ||H >=Hmin))
                detection = 1;
            else
                detection = 0;

            /////////// Saturation /////////////////////
            if(detection){
                if(Smin <=S && S <=Smax)
                    detection = 1;
                else if (Smax <Smin && (S <=Smax ||S >=Smin))
                    detection = 1;
                else
                    detection = 0;
            }

            /////////// Value /////////////////////
            if(detection){
                if(Vmin <=V && V <=Vmax)
                    detection = 1;
                else if (Vmax <Vmin && (V <=Vmax ||V >=Vmin))
                    detection = 1;
                else
                    detection = 0;
            }

            if(maskInversion)
                detection = !detection;
            if(detection)
                pix.val[0]= 0;
            else
                pix.val[0] = 255;
            cvSet2D(grayMaskHSV,i,j,pix);
        }
    }
}


// event handlers
void refresh_hue_min(int changedValue)
{
    Hmin = changedValue;
    segment_HSVimage();
    cvShowImage("mask", grayMaskHSV );
}
void refresh_hue_max(int changedValue)
{
    Hmax = changedValue;
    segment_HSVimage();
    cvShowImage("mask", grayMaskHSV );
}
void refresh_saturation_min(int changedValue)
{
    Smin = changedValue;
    segment_HSVimage();
    cvShowImage("mask", grayMaskHSV );
}
void refresh_saturation_max(int changedValue)
{
    Smax = changedValue;
    segment_HSVimage();
    cvShowImage("mask", grayMaskHSV );
}
void refresh_value_min(int changedValue)
{
    Vmin = changedValue;
    segment_HSVimage();
    cvShowImage("mask", grayMaskHSV );
}
void refresh_value_max(int changedValue)
{
    Vmax = changedValue;
    segment_HSVimage();
    cvShowImage("mask", grayMaskHSV );
}
void refresh_inversion(int changedValue)
{
    maskInversion = changedValue;
    segment_HSVimage();
    cvShowImage("mask", grayMaskHSV );
}
void colorMouseEventHandler(int event, int x, int y, int flags, void *param)
{
    if(event == CV_EVENT_MOUSEMOVE)
    {
        // improve and apply gray level mask on BGR image
        improve_and_apply_grayScaleMask(&grayMaskHSV,imgBGR,&improvedMaskOnBGR);
        cvShowImage("mask applied", improvedMaskOnBGR);
    }
}

