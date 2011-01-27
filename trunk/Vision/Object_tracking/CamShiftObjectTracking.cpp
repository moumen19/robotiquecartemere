#include "CamShiftObjectTracking.hpp"


void onMouse( int event, int x, int y, int, void* )
{
    if( CamShiftObjectTracking::selectObject )
    {
        CamShiftObjectTracking::selection.x = MIN(x, CamShiftObjectTracking::origin.x);
        CamShiftObjectTracking::selection.y = MIN(y, CamShiftObjectTracking::origin.y);
        CamShiftObjectTracking::selection.width = std::abs(x - CamShiftObjectTracking::origin.x);
        CamShiftObjectTracking::selection.height = std::abs(y - CamShiftObjectTracking::origin.y);
        CamShiftObjectTracking::selection &= Rect(0, 0, CamShiftObjectTracking::image.cols, CamShiftObjectTracking::image.rows);
    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        CamShiftObjectTracking::origin = Point(x,y);
        CamShiftObjectTracking::selection = Rect(x,y,0,0);
        CamShiftObjectTracking::selectObject = true;
        break;
    case CV_EVENT_LBUTTONUP:
        CamShiftObjectTracking::selectObject = false;
        if( CamShiftObjectTracking::selection.width > 0 && CamShiftObjectTracking::selection.height > 0 )
            CamShiftObjectTracking::trackObject = -1;
        break;
    }
}



CamShiftObjectTracking::CamShiftObjectTracking()
{
    //ctor
}

CamShiftObjectTracking::~CamShiftObjectTracking()
{
    //dtor
}

void CamShiftObjectTracking::help()
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


void CamShiftObjectTracking::Setup()
{
    cap.open(0);

    if( !cap.isOpened() )
    {
    	help();
        cout << "***Could not initialize capturing...***\n";
    }

    /// Variables initialisation
    backprojMode = false;
    selectObject = false;
    trackObject = 0;
    showHist = true;
    vmin = 10; vmax = 256; smin = 30;

    badFrameDetected =false;
    Rect trackWindow;
    Rect previousGoodWindow;
    RotatedRect trackBox;
    hsize = 16;
    hranges[0] = 0 ; hranges[1] = 180;
    /***/ phranges = hranges;

    histimg = Mat::zeros(200, 320, CV_8UC3);
    ///

    help();

    namedWindow( "Histogram", 1 );
    namedWindow( "CamShift Demo", 1 );
    setMouseCallback( "CamShift Demo", onMouse, 0 );
    createTrackbar( "Vmin", "CamShift Demo", &vmin, 256, 0 );
    createTrackbar( "Vmax", "CamShift Demo", &vmax, 256, 0 );
    createTrackbar( "Smin", "CamShift Demo", &smin, 256, 0 );
}

// The main loop
void CamShiftObjectTracking::GetObjectCenter(int &center)
{
    Mat frame;
    cap >> frame;
    if( frame.empty() )
        break;

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

        cout<<"trackWindow"<<"(x,y:"<<trackWindow.x<<";"<<trackWindow.y << ")  " <<trackWindow.width<<" x "<<trackWindow.height<<endl;
///////////////////////////////////////////////////////////////////////////////////////////////////////
        RotatedRect trackBox = CamShift(backproj, trackWindow,
                            TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));

        if( backprojMode )
            cvtColor( backproj, image, CV_GRAY2BGR );

        cout<< "center ("<<trackBox.center.x<<";"<<trackBox.center.y << ")"
        << "  size" <<(int)trackBox.size.width<<" ; "<<(int)trackBox.size.height <<endl;

        if(trackBox.size.height>=0 && trackBox.size.width >=0 )
            ellipse( image, trackBox, Scalar(0,255,0), 4, CV_AA );
    }

    if(badFrameDetected){
        int x=previousGoodWindow.x + previousGoodWindow.width/2;
        int y=previousGoodWindow.y  + previousGoodWindow.height/2;
        if(x > 640 ) x = 640;
        if(y > 480 ) y = 480;
//              if(trackWindow.width > 640 ) trackWindow.width = 640;
//               if(trackWindow.height > 480 ) trackWindow.height = 480;
        circle(image, Point( x , y ), 22, Scalar(0,0,255), 3);
        //circle(image, Point(0, 0), 10, Scalar(0,0,255), 3);
        badFrameDetected = false;
    }

    if( selectObject && selection.width > 0 && selection.height > 0 )
    {
        Mat roi(image, selection);
        bitwise_not(roi, roi);
    }

    imshow( "CamShift Demo", image );
    imshow( "Histogram", histimg );

    char c = (char)waitKey(10);
    if( c == 27 )
        break;
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
}
