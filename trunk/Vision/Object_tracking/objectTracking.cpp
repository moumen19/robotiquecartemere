
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

void helpTrack()
{
    cout << "\nThis is a demo that shows mean-shift based tracking\n"
         <<   "You select a color objects such as your face and it tracks it.\n"
         <<   "This reads from video camera (0 by default, or the camera number the user enters\n"
         << "Call:\n"
         << "\n./camshiftdemo [camera number]"
         << "\n" << endl;

    cout << "\n\nHot keys: \n"
        "\tESC - quit the program\n"
        "\tc - stop the tracking\n"
        "\tb - switch to/from backprojection view\n"
        "\th - show/hide object histogram\n"
        "To initialize tracking, select the object with mouse\n" << endl;
}

Mat image;
bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
Point origin;
Rect selection;
int vmin = 10, vmax = 256, smin = 30;
int regionAcquisition = 0;
bool regionIsAcquired = false;

bool badFrameDetected =false;
VideoCapture cap;
Rect trackWindow;
Rect previousGoodWindow;
RotatedRect trackBox;
int hsize = 16;
float hranges[] = {0,180};
const float* phranges = hranges;
Mat hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
Size referenceSize;
bool referenceIsAcquired = false;

void onMouse( int event, int x, int y, int, void* )
{
    if( selectObject )
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);

        selection &= Rect(0, 0, image.cols, image.rows);
    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = Point(x,y);
        selection = Rect(x,y,0,0);
        selectObject = true;
        break;
    case CV_EVENT_LBUTTONUP:
        selectObject = false;
        if( selection.width > 0 && selection.height > 0 ){
            trackObject = -1;
            if(!regionIsAcquired) regionIsAcquired= true;
            referenceIsAcquired = false;
        }
        break;
    }
}


void setupTracking()
{
    cap.open(0);

    if( !cap.isOpened() )
    {
        helpTrack();
        cout << "***Could not initialize capturing...***\n";
    }

    void helpTrack();

    namedWindow( "Histogram", 1 );
    namedWindow( "CamShift Demo", 1 );
    setMouseCallback( "CamShift Demo", onMouse, 0 );
    createTrackbar( "Vmin", "CamShift Demo", &vmin, 256, 0 );
    createTrackbar( "Vmax", "CamShift Demo", &vmax, 256, 0 );
    createTrackbar( "Smin", "CamShift Demo", &smin, 256, 0 );
}


bool Routine(int &center)
{

    Mat frame;
    cap >> frame;
    if( frame.empty() )
        cerr<<"Bad frame ! \n";

    frame.copyTo(image);
    cvtColor(image, hsv, CV_BGR2HSV);

    if( trackObject )
    {

        int _vmin = vmin, _vmax = vmax;

        inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                Scalar(180, 256, MAX(_vmin, _vmax)), mask);
        int ch[] = {0, 0};
        hue.create(hsv.size(), hsv.depth());
        mixChannels(&hsv, 1, &hue, 1, ch, 1);

        if( trackObject < 0 )
        {
            Mat roi(hue, selection), maskroi(mask, selection);
            calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
            normalize(hist, hist, 0, 255, CV_MINMAX);

            trackWindow = selection;
            trackObject = 1;

            histimg = Scalar::all(0);
            int binW = histimg.cols / hsize;
            Mat buf(1, hsize, CV_8UC3);
            for( int i = 0; i < hsize; i++ )
                buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
            cvtColor(buf, buf, CV_HSV2BGR);

            for( int i = 0; i < hsize; i++ )
            {
                int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
                rectangle( histimg, Point(i*binW,histimg.rows),
                           Point((i+1)*binW,histimg.rows - val),
                           Scalar(buf.at<Vec3b>(i)), -1, 8 );
            }
        }

        calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
        backproj &= mask;
///////////////////////////////////////////////////////////////////////////////////////////////////////
        if(trackWindow.x < 0 || trackWindow.y < 0 ||
            trackWindow.x > frame.cols || trackWindow.y > frame.rows ||
            trackWindow.x > 640 || trackWindow.y > 480 ||
            trackWindow.width > frame.cols || trackWindow.height > frame.rows){

            trackWindow = previousGoodWindow;
            cerr<<"Bad track window detected !!"<<endl;

            badFrameDetected = true; // pour afficher dernier
        }
        else{
            previousGoodWindow = trackWindow;
        }

        //cout<<"trackWindow"<<"(x,y:"<<trackWindow.x<<";"<<trackWindow.y << ")  " <<trackWindow.width<<" x "<<trackWindow.height<<endl;
///////////////////////////////////////////////////////////////////////////////////////////////////////
        RotatedRect trackBox = CamShift(backproj, trackWindow,
                            TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));

        if( backprojMode )
            cvtColor( backproj, image, CV_GRAY2BGR );

//            cout<< "center ("<<trackBox.center.x<<";"<<trackBox.center.y << ")"
//            << "  size" <<(int)trackBox.size.width<<" ; "<<(int)trackBox.size.height <<endl;

        if(trackBox.size.height>=0 && trackBox.size.width >=0 )
            ellipse( image, trackBox, Scalar(0,255,0), 4, CV_AA );
    }

    if(regionIsAcquired){

        if(regionAcquisition < 20 ){
            regionAcquisition++;
            //cout<<"detected region: "<<previousGoodWindow.width <<" ; "<<previousGoodWindow.height<<endl;
        }
        else{
            // Here previousGoodFrame is the last good one, so we send it to the robot
            center = previousGoodWindow.x + previousGoodWindow.width/2;

            // define the reference region size
            if(!referenceIsAcquired){
                referenceIsAcquired=true;
                referenceSize.width = previousGoodWindow.width;
                referenceSize.height = previousGoodWindow.height;
            }

            int diffW = referenceSize.width - previousGoodWindow.width;
            int diffH = referenceSize.height  - previousGoodWindow.height;


            if(diffW >=0 && diffW< 10 ){
                cout<<"++ low";
            }
            else if(diffW> 10 && diffW<= 20){
                cout<<"++ middle";
            }
            else if(diffW> 20){
                cout<<"++ high";
            }
            else if(diffW> -10 && diffW <=0 ){
                cout<<"-- low";
            }
            else if(diffW> -20 && diffW<= -10){
                cout<<"-- middle";
            }
            else if(diffW<=-20){
                cout<<"-- high";
            }
            cout<<endl;

//            cout<<"selection: "<<selection.width<<" ; "<<selection.height<<endl;
//            cout<<"difference: "<<" ; "<<
        }

    }

    if(badFrameDetected){
        int x=previousGoodWindow.x;
        int y=previousGoodWindow.y;
        if(x > 640 ) x = 640;
        if(y > 480 ) y = 480;
//              if(trackWindow.width > 640 ) trackWindow.width = 640;
//               if(trackWindow.height > 480 ) trackWindow.height = 480;
        circle(image, Point( x , y ), 22, Scalar(0,0,255), 3);
        //circle(image, Point(0, 0), 10, Scalar(0,0,255), 3);

    }

    if( selectObject && selection.width > 0 && selection.height > 0 )
    {
        Mat roi(image, selection);
        bitwise_not(roi, roi);
    }

    imshow( "CamShift Demo", image );
    imshow( "Histogram", histimg );

    char c = (char)waitKey(10);

    switch(c)
    {
    case 'b':
        backprojMode = !backprojMode;
        break;
    case 'c':
        trackObject = 0;
        histimg = Scalar::all(0);
        break;
    case 'h':
        showHist = !showHist;
        if( !showHist )
            destroyWindow( "Histogram" );
        else
            namedWindow( "Histogram", 1 );
        break;
    default:
        ;
    }

    if(badFrameDetected){
        badFrameDetected = false;
        return false;
    }
    else
        return true;

}


