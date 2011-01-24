#include "GenericImageProcessing.hpp"

 //ctor
GenericImageProcessing::GenericImageProcessing()
{

}

//dtor
GenericImageProcessing::~GenericImageProcessing()
{

}

cv::Mat GenericImageProcessing::Canny(cv::Mat image, bool displaySettingsActivated)
{
    cv::Mat frame;

    if(image.channels() > 1)
        cv::cvtColor(image, frame, CV_BGR2GRAY);
    else
        image.copyTo(frame);

    // sigma = 1.5
    // ksize = (sigma * 5)|1 = 7
    cv::GaussianBlur(frame, frame, cv::Size(7,7), 1.5, 1.5);
    cv::Canny(frame, frame, 0, 30, 3);

    if(displaySettingsActivated){
        cv::namedWindow( "Canny", CV_WINDOW_AUTOSIZE);
        cv::imshow( "Canny", frame );
        cv::waitKey(0);
    }
    return frame;
}

cv::Mat GenericImageProcessing::ColorSegmentation(cv::Mat imageToSegment, cv::Scalar colorToFind, bool displaySettingsActivated)
{
    int colorConfiguration = 0;
    int  Hmin, Hmax, Smin, Smax, Vmin, Vmax;

    // red
   if(colorConfiguration==0){
            Hmin=172;
            Hmax=3;
            Smin=113;
            Smax=237;
            Vmin = 0;
            Vmax=255;
    }
    // green
    if(colorConfiguration==1){
            Hmin=32;
            Hmax=71;
            Smin=66;
            Smax=242;
            Vmin = 0;
            Vmax=255;
    }
    // blue
    if(colorConfiguration==2){
            Hmin=108;
            Hmax=118;
            Smin=99;
            Smax=201;
            Vmin= 80;
            Vmax=255;
    }

    CvScalar pix = cvScalar(0,0,0,0);
    int i,j,detection;
    int H=0,S=0,V=0;

    // Method to convert from the C++ API to the C one
    IplImage imageConvertedFromMat = imageToSegment; // Warning ! However, if the data of imageToSegement are lost, so will be those of imgBGR, as it is only a copy of the header !
    IplImage * imgBGR = &imageConvertedFromMat;
    IplImage * imgHSV = cvCloneImage(imgBGR);
    //////////////////////////////////////////////////

    cvCvtColor(imgBGR, imgHSV, CV_BGR2HSV);
    IplImage * grayMaskHSV = cvCreateImage(cvSize(imgHSV->width, imgHSV->height), imgHSV->depth, 1);
    IplImage * improvedMaskOnBGR = cvCloneImage(imgBGR);
    int maskInversion = 1;

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
    return cv::Mat(grayMaskHSV);
}

// can be used with cv::getStructuringElement to specify the morphologic element
cv::Mat GenericImageProcessing::MorphologyEx(cv::Mat binaryImage, int operation, const cv::Mat& element, cv::Point anchor, int iterations)
{
    if (binaryImage.channels() > 1) {
        cerr<< "Source must be a binary image for MorphologyEx !\n"; exit(1);
    }

    cv::Mat result;

    cv::morphologyEx( binaryImage, result, operation, element, anchor, iterations);

    return result;
}

