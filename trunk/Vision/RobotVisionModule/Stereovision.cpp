#include "Stereovision.hpp"

//constructor
Stereovision::Stereovision():m_LeftCamera(NULL),m_RightCamera(NULL),
                            m_videoOutputCount(0),m_imageOutputCount(0),m_videoBeingRecorded (false)
{
    m_LeftImageBuffer.clear();
    m_RightImageBuffer.clear();

}

//destructor
Stereovision::~Stereovision()
{
    delete m_LeftCamera;
    delete m_RightCamera;
}


//To reimplement
void Stereovision::StereoCalibrate()
{

    int i;
    int ballsPixelsSizes_Left[3] = {0,0,0};
    int ballsPixelsSizes_Right[3] = {0,0,0};
    float fx_Left=0,fy_Left=0,fx_Right=0,fy_Right=0;
    float Z_Left [3][2]= {{0,0},{0,0},{0,0}}, Z_Right [3][2]= {{0,0},{0,0},{0,0}};
    int board_w = 6; // Board width in squares
	int board_h = 8; // Board height
    int n_boards = 15; // Number of boards
	int board_n = board_w * board_h;
	CvSize image_Size;



    image_Size.width = m_image_size.width;
    image_Size.height = m_image_size.height;

    std::cout << "image Size" << m_image_size.width << m_image_size.height << endl;
    printf("image size %d %d\n", image_Size.width, image_Size.height );


    CvMat * intrinsicMatrix_Left  = cvCreateMat(3,3,CV_32FC1);
    CvMat * intrinsicMatrix_Right  = cvCreateMat(3,3,CV_32FC1);
    CvMat * distortionCoeffs_Left = cvCreateMat(4,1,CV_32FC1); // only 4 coeffs will be used (5th = 0)
    CvMat * distortionCoeffs_Right = cvCreateMat(4,1,CV_32FC1);
    CvMat * nbTotalCorners = cvCreateMat(nbImages,1,CV_32SC1); // array containing the corners count for each picture
    CvMat * chessboardplanCoordinates = cvCreateMat(nbImages*nbCorners,3,CV_32FC1); // 3D: x, y, z=0
    CvMat * cornersMat_Left = cvCreateMat(nbImages*nbCorners,2,CV_32FC1);   // 2D: x, y
    CvMat * cornersMat_Right = cvCreateMat(nbImages*nbCorners,2,CV_32FC1);



    CvMat chessboardplanCoordinates_temp = m_chessboardplanCoordinates;
    chessboardplanCoordinates = &chessboardplanCoordinates_temp;

    CvMat cornersMat_Left_temp = m_cornersMat_Left;
    cornersMat_Left = &cornersMat_Left_temp;

    CvMat cornersMat_Right_temp = m_cornersMat_Right;
    cornersMat_Right = &cornersMat_Right_temp;

    CvMat nbTotalCorners_temp = m_nbTotalCorners;
    nbTotalCorners = &nbTotalCorners_temp;


    CvMat intrinsicMatrix_Left_temp = m_intrinsecMatrix_left;
    intrinsicMatrix_Left = &intrinsicMatrix_Left_temp;

    CvMat distortionCoeffs_Left_temp = m_distortionMatrix_left;
    distortionCoeffs_Left = &distortionCoeffs_Left_temp;

    CvMat distortionCoeffs_Right_temp = m_distortionMatrix_right;
    distortionCoeffs_Right = &distortionCoeffs_Right_temp;

    CvMat * RotationMatrix;// = cvCreateMat(3,3,CV_32FC1); // Matrix
    CvMat * TranslationMatrix;// = cvCreateMat(3,1,CV_32FC1); // Vector
    CvMat * EssentialMatrix = 0;// = cvCreateMat(3,3,CV_32FC1);
    CvMat * FundamentalMatrix = 0;// = cvCreateMat(3,3,CV_32FC1);

    std::cout << "test2" << std::endl;


    // Stereovision
    cvStereoCalibrate (chessboardplanCoordinates,
    cornersMat_Left,
    cornersMat_Right,
    nbTotalCorners,
    intrinsicMatrix_Left,
    distortionCoeffs_Left,
    intrinsicMatrix_Left,
    distortionCoeffs_Right,
    image_Size,
    RotationMatrix,
    TranslationMatrix,
    EssentialMatrix,
    FundamentalMatrix,

    //cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 100, 1e-5), // default
    cvTermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 30, 1e-6),

    //CV_CALIB_FIX_INTRINSIC //can be used if previous cameras calibration was accurate (as in this example)
    CV_CALIB_USE_INTRINSIC_GUESS // can be used to refine previous intrinsic/distortion parameters
    );


    std::cout << "Success: StereoCalibration..." << std::endl;
}

// To implement
bool Stereovision::SaveMatrix()
{

}

// To implement
bool Stereovision::LoadStereoMatrices(const string &filename)
{

}

// connects sources, external..
void Stereovision::Setup(int mode)
{
    if(mode == CAMERA_MODE){  // setup from camera
        // left
        m_LeftCamera = new Camera(0);
        if(!m_LeftCamera->isOpened()){
            cerr<<"Fail to open the left camera !"<<endl;
            exit(1);
        }
        // right
        m_RightCamera = new Camera(1);
        if(!m_RightCamera->isOpened()){
            cerr<<"Fail to open the right camera !"<<endl;
            exit(1);
        }
    }
    else if(mode == VIDEO_FILE_MODE){ // setup from file

        // left
        m_LeftCamera = new Camera("//host//TRAVAIL//M2//Vision//Echantillons//stereo_videos//ground1//Vid_Left_1.avi");
        if(!m_LeftCamera->isOpened()){
            cerr<<"Fail to open the left video !"<<endl;
            exit(1);
        }
        // right
        m_RightCamera = new Camera("//host//TRAVAIL//M2//Vision//Echantillons//stereo_videos//ground1//Vid_Right_1.avi");
        if(!m_RightCamera->isOpened()){
            cerr<<"Fail to open the right video !"<<endl;
            exit(1);
        }
    }
    else{
        cerr<<"\nNo video source specified !"; exit(1);
    }
    cout<<"Sources initialised successfuly\n";
}

// main routine of the video processing module
void Stereovision::Run()
{

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

// push them into buffers ?
//    m_LeftImageBuffer.push_back(frameL);
//    m_RightImageBuffer.push_back(frameR);
    return true;
}

// Same function as LiveDisplay() in Camera.hpp
void Stereovision::RawDisplay()
{
    char keyPressed = -1;
    cv::namedWindow( "RawDisplay_Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "RawDisplay_Right", CV_WINDOW_AUTOSIZE);

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
        cv::imshow( "Canny Left", m_LeftFrame );
        cv::imshow( "Canny Right", m_RightFrame );
    }
}

// Simple Canny display
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

// for now the seed is hard-coded at the bottom of the image (cf initialisation)
// this implementation is focused only on 1 camera
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
        cv::Mat can = Canny(m_LeftFrame, false); // the mask

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

void Stereovision::BlobTracking(cv::Scalar colorToTrack)
{
    char keyPressed = -1;
    cv::namedWindow( "BlobTracking_Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "BlobTracking_Right", CV_WINDOW_AUTOSIZE);
    cv::Mat SegmentedLeft, SegmentedRight;

    while(true)
    {
        // takes one frame of each Camera
        if(!AcquireFrames()) break;

        ///////////////* Color Segmentation *//////////////////////////////////
        SegmentedLeft = ColorSegmentation(m_LeftFrame, colorToTrack, false);
        SegmentedRight = ColorSegmentation(m_RightFrame, colorToTrack, false);

        // improvment ?
        SegmentedLeft = MorphologyEx(SegmentedLeft, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1,-1), 3 );
        ///////////////////////////////////////////////////////////////////////

        keyPressed = cv::waitKey(WAITING_TIME_MS);
        if(keyPressed != -1){   // if a key is pressed,
            if(!GetUserInputs(keyPressed)) break; // get the key value
        }

        if(m_videoBeingRecorded){ // recording video
           m_LeftVideoWriter << SegmentedLeft;
           m_RightVideoWriter << SegmentedRight;
           cout<< "recording vid ..." <<endl;
        }

        // display
        cv::imshow( "BlobTracking_Left", SegmentedLeft );
        cv::imshow( "BlobTracking_Right", SegmentedRight );
    }
}

// used for tests with corners detection/matching
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

// simple implementation of cv::Canny()
cv::Mat Stereovision::Canny(cv::Mat image, bool displaySettingsActivated)
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

// simple implementation of cv::MorphologyEx()
// can be used with cv::getStructuringElement to specify the morphologic element
cv::Mat Stereovision::MorphologyEx(cv::Mat binaryImage, int operation, const cv::Mat& element, cv::Point anchor, int iterations)
{
    if (binaryImage.channels() > 1) {
        cerr<< "Source must be a binary image for MorphologyEx !\n"; exit(1);
    }
    cv::Mat result;
    cv::morphologyEx( binaryImage, result, operation, element, anchor, iterations);
    return result;
}

// need to be improved: needs better adjustable thresolds, open/close on the binary mask...
cv::Mat ColorSegmentation(cv::Mat imageToSegment, cv::Scalar colorToFind, bool displaySettingsActivated)
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

// for corners detection/matching
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
    cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create( "SURF" );
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

        cout<<keypoints1.size();

        descriptorExtractor->compute(grayScaleL,keypoints1,descriptors1);
        descriptorExtractor->compute(grayScaleR,keypoints2,descriptors2);

        cv::Mat drawImg;
        vector<cv::DMatch> filteredMatches;
        filteredMatches.clear();
        crossCheckMatching( descriptorMatcher, descriptors1, descriptors2, filteredMatches, 2 );
        cv::drawMatches( grayScaleL, keypoints1, grayScaleR, keypoints2, filteredMatches, drawImg );
        cv::waitKey(0);

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

// used for tests with corners detection/matching
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

