#include "Stereovision.hpp"

//ctor
Stereovision::Stereovision():m_LeftCamera(NULL),m_RightCamera(NULL)
{
    m_LeftImageBuffer.clear();
    m_RightImageBuffer.clear();
}

//dtor
Stereovision::~Stereovision()
{
    delete m_LeftCamera;
    delete m_RightCamera;
}

// connect sources, external..
void Stereovision::Setup()
{
    try{
        // setup left camera
        m_LeftCamera = new Camera("webcam1.avi");
        //m_LeftCamera = new Camera(0);
        if(!m_LeftCamera->isOpened()){
            cerr<<"Fail to open the left camera !"<<endl;
            throw (new std::exception());
        }

        // setup right camera
        m_RightCamera = new Camera("webcam2.avi");
        //m_RightCamera = new Camera(1);
        if(!m_RightCamera->isOpened()){
            cerr<<"Fail to open the right camera !"<<endl;
            throw (new std::exception());
        }

    }
    catch(std::exception e){
        cerr<<e.what(); exit(1);
    }
}


// display both cameras untouched (same as LiveDisplay in class Camera)
void Stereovision::RawDisplay()
{
    cv::Mat frameL, frameR;
    cv::namedWindow( "rawDisplay_Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "rawDisplay_Right", CV_WINDOW_AUTOSIZE);

    while(cv::waitKey(20) < 1)
    {
        // left
        *m_LeftCamera >> frameL;
        if(frameL.empty()) break;

        // right
        *m_RightCamera >> frameR;
        if(frameR.empty())break;

        cv::imshow( "rawDisplay_Left", frameL );
        cv::imshow( "rawDisplay_Right", frameR );
    }
}


void Stereovision::CannyEdgeDetection()
{
    cv::Mat frameL, frameR;
    cv::namedWindow( "Canny Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "Canny Right", CV_WINDOW_AUTOSIZE);

    while(cv::waitKey(20) < 1)
    {
        // left
        *m_LeftCamera >> frameL;
        if(frameL.empty()) break;
        m_LeftImageBuffer.push_back(frameL);

        //////////*     Core Algorithm           *//////////////////
        cv::cvtColor(m_LeftImageBuffer.back(), frameL, CV_BGR2GRAY);
        // sigma = 1.5
        // ksize = (sigma * 5)|1 = 7
        //cv::GaussianBlur(frameL, frameL, cv::Size(7,7), 1.5, 1.5);
        //cv::Canny(frameL, frameL, 0, 30, 3);
        cv::Sobel(frameL,frameL,frameL.depth(),2,2);
        /////////////////////////////////////////////////////////////

        // right
        *m_RightCamera >> frameR;
        if(frameR.empty())break;

//        m_RightImageBuffer.push_back(frameR);
//        cv::cvtColor(m_RightImageBuffer.back(), frameR, CV_BGR2GRAY);
//        cv::GaussianBlur(frameR, frameR, cv::Size(7,7), 1.5, 1.5);
//        cv::Canny(frameR, frameR, 0, 30, 3);

        cv::imshow( "Canny Left", frameL );
        cv::imshow( "Canny Right", frameR );
    }
}

// main routine of the video processing module
void Stereovision::Run()
{
    cv::Mat frameL, frameR; // untouched

    cv::namedWindow( "rawDisplay_Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "rawDisplay_Right", CV_WINDOW_AUTOSIZE);

    // normal image stored on the right
    *m_LeftCamera >> frameL;
    *m_RightCamera >> frameR;
    m_LeftImageBuffer.push_back(frameL);
    m_RightImageBuffer.push_back(m_LeftImageBuffer.back());

    cv::cvtColor(m_LeftImageBuffer.back(), frameL, CV_BGR2GRAY);
    m_LeftImageBuffer.push_back(frameL);

    cv::Mat bw = frameR < 100;
////////////////////////////////////////////////////////////
    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;

    cv::findContours( m_LeftImageBuffer.back(), contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	cv::Mat dst = cv::Mat::zeros(m_LeftImageBuffer.back().size(), CV_8UC3);

    if( contours.size() > 0 )
    {
        // iterate through all the top-level contours,
        // draw each connected component with its own random color
        int idx = 0;
        for( ; idx >= 0; idx = hierarchy[idx][0] )
        {
            cv::Scalar color( (rand()&255), (rand()&255), (rand()&255) );
            cv::drawContours( dst, contours, idx, color, CV_FILLED, 8, hierarchy );
        }
    }
    m_RightImageBuffer.push_back(frameR);
	////////////////////////////////////////////////////////////

    cv::imshow( "rawDisplay_Left", m_LeftImageBuffer.back() );
    cv::imshow( "rawDisplay_Right", bw );

    cout<<endl<<"nb images Left: "<<m_LeftImageBuffer.size()<<endl;
    cout<<endl<<"nb images Right: "<<m_RightImageBuffer.size()<<endl;
   // if(contours.size() >0) cout<<endl<<"contours:"<<contours.back().size();

    cv::waitKey(0);
}

void Stereovision::test()
{
    cv::Mat img1raw, img2raw;
    cv::Mat img1, img2;

    // left
    *m_LeftCamera >> img1raw;
    *m_RightCamera>> img2raw;


    cv::cvtColor(img1raw, img1, CV_BGR2GRAY);
    cv::cvtColor(img2raw, img2, CV_BGR2GRAY);

    // detecting keypoints
    //cv::FastFeatureDetector detector;
    //cv::SiftFeatureDetector detector;
	cv::SurfFeatureDetector detector(10000);
	vector<cv::KeyPoint> keypoints1, keypoints2;
	detector.detect(img1, keypoints1);
	detector.detect(img2, keypoints2);

	// computing descriptors
	cv::SurfDescriptorExtractor extractor;
	//cv::SiftDescriptorExtractor extractor;
	//cv::SurfDescriptorExtractor extractor;
	cv::Mat descriptors1, descriptors2;
	extractor.compute(img1, keypoints1, descriptors1);
	extractor.compute(img2, keypoints2, descriptors2);

	// matching descriptors
	cv::BruteForceMatcher<cv::L2<float> > matcher;
    vector<cv::DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

	// drawing the results
	cv::namedWindow("matches", 1);
	cv::Mat img_matches;
	cv::drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches);
	cv::imshow("matches", img_matches);
	cv::waitKey(0);

}
