#include "Stereovision.hpp"

//ctor
Stereovision::Stereovision():m_LeftCamera(NULL),m_RightCamera(NULL)
{
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
        if(!m_LeftCamera->isOpened())
           throw (new std::exception());

        // setup right camera
        m_RightCamera = new Camera("webcam2.avi");
        //m_RightCamera = new Camera(1);
        if(!m_RightCamera->isOpened())
            throw (new std::exception());

    }
    catch(std::exception e){
        cerr<<e.what(); exit(1);
    }
}


// display both cameras untouched
void Stereovision::RawVideoDisplay()
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

// main routine of the video processing module
void Stereovision::Run()
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
    cv::FastFeatureDetector detector;
    //cv::SiftFeatureDetector detector;
	//cv::SurfFeatureDetector detector(10000);
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
