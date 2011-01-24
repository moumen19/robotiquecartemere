#include "Stereovision.hpp"

//ctor
Stereovision::Stereovision():m_LeftCamera(NULL),m_RightCamera(NULL),
                            m_videoOutputCount(0),m_imageOutputCount(0),m_videoBeingRecorded (false)
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

// connects sources, external..
void Stereovision::Setup(int mode)
{
    try{

        if(mode == CAMERA_MODE){  // setup from camera

            // left
            m_LeftCamera = new Camera(0);
            if(!m_LeftCamera->isOpened()){
                cerr<<"Fail to open the left camera !"<<endl;
                throw (new std::exception());
            }
            // right
            m_RightCamera = new Camera(1);
            if(!m_RightCamera->isOpened()){
                cerr<<"Fail to open the right camera !"<<endl;
                throw (new std::exception());
            }
        }
        else if(mode == VIDEO_FILE_MODE){ // setup from file

            // left
            m_LeftCamera = new Camera("//host//TRAVAIL//M2//Vision//Echantillons//stereo_videos//ground1//Vid_Left_1.avi");
            if(!m_LeftCamera->isOpened()){
                cerr<<"Fail to open the left video !"<<endl;
                throw (new std::exception());
            }
            // right
            m_RightCamera = new Camera("//host//TRAVAIL//M2//Vision//Echantillons//stereo_videos//ground1//Vid_Right_1.avi");
            if(!m_RightCamera->isOpened()){
                cerr<<"Fail to open the right video !"<<endl;
                throw (new std::exception());
            }
        }
        else{
            cerr<<"\nNo video source specified !"; exit(1);
        }
    }
    catch(std::exception e){
        cerr<<e.what(); exit(1);
    }
    cout<<"Sources initialised successfuly\n";
}

// takes one frame of each Camera
bool Stereovision::AcquireFrames()
{
    // left
    *m_LeftCamera >> m_LeftFrame;
    if(m_LeftFrame.empty()) {
        cerr<<"Pb acquisition left frame !"<<endl;
        return false;
    }

    // right
    *m_RightCamera >> m_RightFrame;
    if(m_RightFrame.empty()){
        cerr<<"Pb acquisition right frame !"<<endl;
        return false;
    }

//    m_LeftImageBuffer.push_back(frameL);
//    m_RightImageBuffer.push_back(frameR);
    return true;
}

// called when a key is pressed, returns 'false' if exit is required
bool Stereovision::GetUserInputs(char key)
{
       // Exit loop
        if(key == STOP_DISPLAY_KEY) return false;

        // Image capture : 'i'
        if(key == 105){
            stringstream convert;
            convert << m_imageOutputCount++ ;
            imwrite(string("Images/Img_left_") + convert.str() + string(".jpg"), m_LeftFrame); // Writes the image
            imwrite(string("Images/Img_right_") + convert.str() + string(".jpg"), m_RightFrame); // Writes the image
        }
        // Video capture : 'v'
        if(key == VIDEO_RECORD_KEY){
                if(!m_videoBeingRecorded){
                    m_videoBeingRecorded = true; // starts recording

                    stringstream convert;
                    convert << m_videoOutputCount++ ;
                    // Opens the video
                    m_LeftVideoWriter.open(string("Videos/Vid_Left_")+ convert.str() +string(".avi"), CV_FOURCC('M', 'J', 'P', 'G'), 20, m_LeftFrame.size());
                    m_RightVideoWriter.open(string("Videos/Vid_Right_")+ convert.str() +string(".avi"), CV_FOURCC('M', 'J', 'P', 'G'), 20,  m_RightFrame.size());
                    if(!m_LeftVideoWriter.isOpened() || !m_RightVideoWriter.isOpened()) { cerr<<"Error trying to open the video writer!" <<endl; return false;}
                }
                else{
                    m_videoBeingRecorded = false; // stops recording
                }

        }

        return true;
}

// display both cameras untouched (same as LiveDisplay in class Camera)
void Stereovision::RawDisplay()
{
    char keyPressed = -1;
    cv::namedWindow( "rawDisplay_Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "rawDisplay_Right", CV_WINDOW_AUTOSIZE);

    while(true)
    {
        // takes one frame of each Camera
        if(!AcquireFrames()) break;

        keyPressed = cv::waitKey(WAITING_TIME_MS);
        if(keyPressed != -1){   // if a key is pressed,
            if(!GetUserInputs(keyPressed)) break; // get the key value
        }

        if(m_videoBeingRecorded){ // recording video
           m_LeftVideoWriter << m_LeftFrame;
           m_RightVideoWriter << m_RightFrame;
           cout<< "recording vid ..." <<endl;
        }

        // display
        cv::imshow( "rawDisplay_Left", m_LeftFrame );
        cv::imshow( "rawDisplay_Right", m_RightFrame );
    }
}

void Stereovision::CannyEdgeDetection()
{
    float sigma = 2.5;
    float ksize = 15 ;

    char keyPressed = -1;
    cv::namedWindow( "Canny Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "Canny Right", CV_WINDOW_AUTOSIZE);

    while(true)
    {
        // takes one frame of each Camera
        if(!AcquireFrames()) break;

        //////////*     Core Algorithm           *//////////////////
        m_LeftImageBuffer.push_back(m_LeftFrame);
        cv::cvtColor(m_LeftImageBuffer.back(), m_LeftFrame, CV_BGR2GRAY);
        cv::GaussianBlur(m_LeftFrame, m_LeftFrame, cv::Size(ksize,ksize), sigma, sigma);
        cv::Canny(m_LeftFrame, m_LeftFrame, 0, 32, 3);

        m_RightImageBuffer.push_back(m_RightFrame);
        cv::cvtColor(m_RightImageBuffer.back(), m_RightFrame, CV_BGR2GRAY);
        cv::GaussianBlur(m_RightFrame, m_RightFrame, cv::Size(ksize,ksize), sigma, sigma);
        cv::Canny(m_RightFrame, m_RightFrame, 0, 32, 3);
        /////////////////////////////////////////////////////////////

        keyPressed = cv::waitKey(WAITING_TIME_MS);
        if(keyPressed != -1){   // if a key is pressed,
            if(!GetUserInputs(keyPressed)) break; // get the key value
        }

        if(m_videoBeingRecorded){ // recording video
           cv::Mat l,r;
           cv::cvtColor(m_LeftFrame,l,CV_GRAY2BGR);
           cv::cvtColor(m_RightFrame,r,CV_GRAY2BGR);
           m_LeftVideoWriter << l;
           m_RightVideoWriter << r;
           cout<< "recording vid ..." <<endl;
        }

        // display
        cv::imshow( "Canny Left", m_LeftFrame );
        cv::imshow( "Canny Right", m_RightFrame );
    }
}

void Stereovision::FloodFilling()
{
    char keyPressed = -1;
    cv::namedWindow( "floodFill_Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "floodFill_Right", CV_WINDOW_AUTOSIZE);

    cv::Mat image, gray, mask;
    int ffillMode = 1;
    int loDiff = 60, upDiff = 20;
    int connectivity = 4;
    int isColor = true;
    bool useMask = true;
    int newMaskVal = 255;

    int lo = ffillMode == 0 ? 0 : loDiff;
    int up = ffillMode == 0 ? 0 : upDiff;
    int flags = connectivity + (newMaskVal << 8) + (ffillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);
    int b = (unsigned)cv::theRNG() & 255;
    int g = (unsigned)cv::theRNG() & 255;
    int r = (unsigned)cv::theRNG() & 255;
    cv::Rect ccomp;
    cv::Scalar newVal = isColor ? cv::Scalar(b, g, r) : cv::Scalar(r*0.299 + g*0.587 + b*0.114);
    int area;

    // initialisation
    AcquireFrames();
    cv::Point seed = cv::Point(m_LeftFrame.cols/2 ,m_LeftFrame.rows -10);
    mask.create(m_LeftFrame.rows+2, m_LeftFrame.cols+2, CV_8UC1);



    while(true)
    {
        // takes one frame of each Camera
        if(!AcquireFrames()) break;

        ///////////////////////////////*             Core Algorithm                             *///////////////////////
        m_LeftFrame.copyTo(image);
        cv::cvtColor(m_LeftFrame, gray, CV_BGR2GRAY);
        cv::Mat dst = isColor ? image : gray;
        cv::Mat can = GenericImageProcessing::Canny(m_LeftFrame, false);

         for(int i = 0; i < can.rows; i++)
            for(int j = 0; j < can.cols; j++){
                mask.at<uchar>(i,j) = 0;
                if(can.at<uchar>(i,j) == 255)
                    mask.at<uchar>(i,j) = 255;
        }

        //mask.reshape(0, m_LeftFrame.rows +2);

        //    cv::rectangle(mask, cv::Point(200,100), cv::Point(300,300), cv::Scalar(b, g, r));
        //    cv::rectangle(mask, cv::Point(350,100), cv::Point(500,300), cv::Scalar(b, g, r));


        if( useMask ) {
            cv::dilate(mask,mask,cv::Mat(),cv::Point(-1,-1),1);
            cv::threshold(mask, mask, 1, 128, CV_THRESH_BINARY);
            area = cv::floodFill(dst, mask, seed, newVal, &ccomp, cv::Scalar(lo, lo, lo), cv::Scalar(up, up, up), flags);
        }
        else{
            area = cv::floodFill(dst, seed, newVal, &ccomp, cv::Scalar(lo, lo, lo), cv::Scalar(up, up, up), flags);
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        keyPressed = cv::waitKey(WAITING_TIME_MS);
        if(keyPressed != -1){   // if a key is pressed,
            if(!GetUserInputs(keyPressed)) break; // get the key value
        }

        if(m_videoBeingRecorded){ // records the video
           m_LeftVideoWriter << dst;
           m_RightVideoWriter << m_RightFrame;
           cout<< "recording vid ..." <<endl;
        }

        // display
        cv::imshow( "floodFill_Left", dst );
        cv::imshow( "floodFill_Right", m_RightFrame );
        cout << area << " pixels were repainted\n";
    }
}

void Stereovision::ProximityMap()
{
    char keyPressed = -1;
    cv::namedWindow( "ProximityMap_Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "ProximityMap_Right", CV_WINDOW_AUTOSIZE);
    cv::Mat sample= cv::imread("//host//TRAVAIL//M2//Vision//Echantillons//matchTemplate//groundSample.jpg", 1);
    cv::Mat result;

    while(true)
    {
        // takes one frame of each Camera
        if(!AcquireFrames()) break;

        cv::matchTemplate( m_LeftFrame, sample, result, CV_TM_SQDIFF);

        keyPressed = cv::waitKey(WAITING_TIME_MS);
        if(keyPressed != -1){   // if a key is pressed,
            if(!GetUserInputs(keyPressed)) break; // get the key value
        }

        if(m_videoBeingRecorded){ // recording video
           m_LeftVideoWriter << result;
           m_RightVideoWriter << m_RightFrame;
           cout<< "recording vid ..." <<endl;
        }

        // display
        cv::imshow( "ProximityMap_Left", result );
        cv::imshow( "ProximityMap_Right", m_RightFrame );
    }
}

// main routine of the video processing module
void Stereovision::Run(){}

void Stereovision::MatchCorners()
{
    char keyPressed = -1;
    cv::namedWindow( "Matches", CV_WINDOW_AUTOSIZE);

    // params for GFTT
    int maxCorners=1000;
    double qualityLevel=0.03;
    double minDistance=3;
    int blockSize=15;
    double k=0.04;
    bool useHarrisDetector= true;
    cv::GoodFeaturesToTrackDetector::Params GFTTparams (maxCorners,
                                qualityLevel, minDistance,
                                 blockSize,
                                useHarrisDetector, k);
    cv::Mat grayScaleL, grayScaleR,out;
    vector<cv::KeyPoint> keypoints1;
    vector<cv::KeyPoint> keypoints2;


    cv::Mat descriptors1;
    cv::Mat descriptors2;

    //cv::GoodFeaturesToTrackDetector detector(GFTTparams);
    cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create( "STAR" );
    cv::Ptr<cv::DescriptorExtractor> descriptorExtractor = cv::DescriptorExtractor::create( "SURF" );
    cv::Ptr<cv::DescriptorMatcher> descriptorMatcher = cv::DescriptorMatcher::create("BruteForce" );

    cv::Mat mask;

    while(true)
    {
        // takes one frame of each Camera
        if(!AcquireFrames()) break;

        ////////////////////////////////////////////////////////
        // cv::Mat() => GFTT; // erosion of Cany !!!

        cv::cvtColor(m_LeftFrame, grayScaleL, CV_BGR2GRAY);
        mask = GenericImageProcessing::Canny(grayScaleL, false);
        //detector.detect(grayScaleL, keypoints1, mask);
        detector->detect(grayScaleL, keypoints1, mask);
        cv::drawKeypoints( grayScaleL, keypoints1, m_LeftFrame);

        cv::cvtColor(m_RightFrame, grayScaleR, CV_BGR2GRAY);
        mask = GenericImageProcessing::Canny(grayScaleR, false);
        //detector.detect(grayScaleR, keypoints2, mask);
        detector->detect(grayScaleR, keypoints2, mask);
        cv::drawKeypoints( grayScaleR, keypoints2, m_RightFrame);

        //cout<<keypoints1.size();

        descriptorExtractor->compute(grayScaleL,keypoints1,descriptors1);
        descriptorExtractor->compute(grayScaleR,keypoints2,descriptors2);

        cv::Mat drawImg;
        vector<cv::DMatch> filteredMatches;
        filteredMatches.clear();
        crossCheckMatching( descriptorMatcher, descriptors1, descriptors2, filteredMatches, 1 );
        cv::drawMatches( grayScaleL, keypoints1, grayScaleR, keypoints2, filteredMatches, drawImg );

        ////////////////////////////////////////////////////////
        keyPressed = cv::waitKey(WAITING_TIME_MS);
        if(keyPressed != -1){   // if a key is pressed,
            if(!GetUserInputs(keyPressed)) break; // get the key value
        }

        if(m_videoBeingRecorded){ // recording video
           m_LeftVideoWriter << drawImg;
           cout<< "recording vid ..." <<endl;
        }

        // display
        cv::imshow( "Matches", drawImg );
    }
}

void Stereovision::crossCheckMatching( cv::Ptr<cv::DescriptorMatcher>& descriptorMatcher,
                         const cv::Mat& descriptors1, const cv::Mat& descriptors2,
                         vector<cv::DMatch>& filteredMatches12, int knn )
{
    filteredMatches12.clear();
    vector<vector<cv::DMatch> > matches12, matches21;
    descriptorMatcher->knnMatch( descriptors1, descriptors2, matches12, knn );
    descriptorMatcher->knnMatch( descriptors2, descriptors1, matches21, knn );
    for( size_t m = 0; m < matches12.size(); m++ )
    {
        bool findCrossCheck = false;
        for( size_t fk = 0; fk < matches12[m].size(); fk++ )
        {
            cv::DMatch forward = matches12[m][fk];

            for( size_t bk = 0; bk < matches21[forward.trainIdx].size(); bk++ )
            {
                cv::DMatch backward = matches21[forward.trainIdx][bk];
                if( backward.trainIdx == forward.queryIdx )
                {
                    filteredMatches12.push_back(forward);
                    findCrossCheck = true;
                    break;
                }
            }
            if( findCrossCheck ) break;
        }
    }
}

void Stereovision::test()
{
    cv::Mat frameL= cv::imread("//host//TRAVAIL//M2//Vision//Echantillons//stereo_images//L0.jpg", 0);
    cv::Mat frameR= cv::imread("//host//TRAVAIL//M2//Vision//Echantillons//stereo_images//R0.jpg", 0);

    int apertureSize = 3;

     // params for GFTT
    int maxCorners=1000;
    double qualityLevel=0.08;
    double minDistance=15;
    int blockSize=7;
    double k=0.04;
    bool useHarrisDetector= true;
    cv::GoodFeaturesToTrackDetector::Params GFTTparams (maxCorners,
                                qualityLevel, minDistance,
                                 blockSize,
                                useHarrisDetector, k);
    cv::Mat out;
    vector<cv::KeyPoint> keypoints1;
    cv::GoodFeaturesToTrackDetector GFTTdetector(GFTTparams);


    cv::Mat mask = GenericImageProcessing::Canny(frameL,false);
    // cv::Mat(); // erosion of Cany !!!


    //cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create( "GFTT" );

    //detector->detect( frameL, keypoints1, mask );
    GFTTdetector.detect( frameL, keypoints1, mask );

    cout<<keypoints1.size();

    cv::drawKeypoints( frameL, keypoints1, out);
    cv::imshow("preCorn",out);
    cv::waitKey(0);


    //    cv::goodFeaturesToTrack( image, cornersVect, maxCorners,
//                            qualityLevel, minDistance,
//                            mask, blockSize,
//                            useHarrisDetector, k );
    //cv::KeyPoint::convert(cornersVect, keypoints, 2, 2, 0, -1);

    //cv::cornerMinEigenVal( image, corners, 5, 3);
    //cv::cornerHarris( image, corners, 9, 3, 5 );
    //cv::cornerEigenValsAndVecs( image, corners, 5, 3);
    //cv::preCornerDetect(image, corners, 3);
    //

//    cv::dilate(corners, dilated_corners, cv::Mat(),cv::Point(-1,-1), 1);
//    cv::Mat corner_mask = corners == dilated_corners;

}
