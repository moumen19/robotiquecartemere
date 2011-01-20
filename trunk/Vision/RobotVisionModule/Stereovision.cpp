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
        /////////// setup leftt camera //////////////
        // from file
         m_LeftCamera = new Camera("//host//TRAVAIL//M2//Vision//Webcam_videos//ground1//Vid_Left_1.avi");
         // from camera
        //m_LeftCamera = new Camera(0);
        if(!m_LeftCamera->isOpened()){
            cerr<<"Fail to open the left camera !"<<endl;
            throw (new std::exception());
        }

        /////////// setup right camera //////////////
        // from file
        m_RightCamera = new Camera("//host//TRAVAIL//M2//Vision//Webcam_videos//ground1//Vid_Right_1.avi");
        // from camera
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
    char keyPressed = -1;
    int videoOutputCount = 0;
    bool videoBeingRecorded = false;
    cv::VideoWriter videoWriterL, videoWriterR;
    cv::Mat frameL, frameR;
    cv::namedWindow( "rawDisplay_Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "rawDisplay_Right", CV_WINDOW_AUTOSIZE);

    while(true)
    {
        // Get key presses value
        keyPressed = cv::waitKey(WAITING_TIME_MS);

        // Exit loop
        if(keyPressed == STOP_DISPLAY_KEY) break;

        // left
        *m_LeftCamera >> frameL;
        if(frameL.empty()) break;

        // right
        *m_RightCamera >> frameR;
        if(frameR.empty())break;

//        char keyPressed = cv::waitKey(20);
//        if(keyPressed == 105){ // key 'i'
//            imwrite(string("left.jpg"), frameL);
//            imwrite(string("right.jpg"), frameR);
//        }
        // Video Capture
        if(keyPressed == VIDEO_RECORD_KEY){
                if(!videoBeingRecorded){
                    videoBeingRecorded = true; // starts recording

                    stringstream convert;
                    convert << videoOutputCount++ ;
                    videoWriterL.open(string("Videos/Vid_Left_")+ convert.str() +string(".avi"), CV_FOURCC('M', 'J', 'P', 'G'), 24, frameL.size()); // Opens the video
                    videoWriterR.open(string("Videos/Vid_Right_")+ convert.str() +string(".avi"), CV_FOURCC('M', 'J', 'P', 'G'), 24, frameR.size());
                    if(!videoWriterL.isOpened() || !videoWriterL.isOpened()) { cerr<<"Error trying to open the video writer!" <<endl; break;}
                }
                else{
                    videoBeingRecorded = false; // stops recording
                }

        }
        if(videoBeingRecorded){
           videoWriterL << frameL;
           videoWriterR << frameR;
           cout<< "recording vids" <<endl;
        }

        cv::imshow( "rawDisplay_Left", frameL );
        cv::imshow( "rawDisplay_Right", frameR );
    }
}

void Stereovision::CannyEdgeDetection()
{
    float sigma = 2.5;
    float ksize = 15 ;

    cv::Mat frameL, frameR;
    cv::namedWindow( "Canny Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "Canny Right", CV_WINDOW_AUTOSIZE);

    while(cv::waitKey(100) < 1)
    {
        // left
        *m_LeftCamera >> frameL;
        if(frameL.empty()) break;
        m_LeftImageBuffer.push_back(frameL);

                // right
        *m_RightCamera >> frameR;
        if(frameR.empty())break;
        m_RightImageBuffer.push_back(frameR);

        //////////*     Core Algorithm           *//////////////////
        cv::cvtColor(m_LeftImageBuffer.back(), frameL, CV_BGR2GRAY);
        cv::GaussianBlur(frameL, frameL, cv::Size(ksize,ksize), sigma, sigma);
        cv::Canny(frameL, frameL, 0, 32, 3);

        cv::cvtColor(m_RightImageBuffer.back(), frameR, CV_BGR2GRAY);
        cv::GaussianBlur(frameR, frameR, cv::Size(ksize,ksize), sigma, sigma);
        cv::Canny(frameR, frameR, 0, 32, 3);
        /////////////////////////////////////////////////////////////

        cv::imshow( "Canny Left", frameL );
        cv::imshow( "Canny Right", frameR );
    }
}

// main routine of the video processing module
void Stereovision::Run()
{
    cv::Mat frameL, frameR;
    cv::namedWindow( "Canny Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "Canny Right", CV_WINDOW_AUTOSIZE);

    while(cv::waitKey(200) < 1)
    {
        *m_LeftCamera >> frameL;
        if(frameL.empty()) break;
        *m_RightCamera >> frameR;
        if(frameR.empty())break;

        m_LeftImageBuffer.push_back(frameL);
        m_RightImageBuffer.push_back(frameR);

        // Edge Detection
        cv::cvtColor(m_LeftImageBuffer.back(), frameL, CV_BGR2GRAY);
        cv::cvtColor(m_RightImageBuffer.back(), frameR, CV_BGR2GRAY);
        cv::GaussianBlur(frameL, frameL, cv::Size(7,7), 1.5, 1.5);
        cv::GaussianBlur(frameR, frameR, cv::Size(7,7), 1.5, 1.5);
        cv::Canny(frameL, frameL, 0, 30, 3);
        cv::Canny(frameR, frameR, 0, 30, 3);

        // Morphological operation
        cv::morphologyEx(frameL,frameL,cv::MORPH_CLOSE,cv::Mat(2,3,CV_8U),cv::Point(-1,-1), 2);
        cv::morphologyEx(frameR,frameR,cv::MORPH_OPEN,cv::Mat(3,3,CV_8U),cv::Point(-1,-1), 2);
        //cv::dilate(frameL,frameL,cv::Mat(2,2,CV_8U));
        //cv::erode(frameL,frameL,cv::Mat(2,2,CV_8U));

        cv::imshow( "Canny Left", frameL );
        cv::imshow( "Canny Right", frameR );
    }
}

void Stereovision::test()
{
    float sigma = 2.5;
    float ksize = 15 ;


    cv::Mat frameL= cv::imread("Images//L0.jpg",1);
    cv::Mat frameR= cv::imread("Images//L0.jpg",1);

    m_LeftImageBuffer.push_back(frameL);
    m_RightImageBuffer.push_back(frameR);

    ///////////////////////////////////////////////////
//    cv::cvtColor(m_RightImageBuffer.back(), frameR, CV_BGR2GRAY);
//    cv::GaussianBlur(frameR, frameR, cv::Size(ksize,ksize), sigma, sigma);
//    cv::Canny(frameR, frameR, 0, 32, 3);
//
//    cv::cvtColor(m_LeftImageBuffer.back(), frameL, CV_BGR2GRAY);
//    cv::GaussianBlur(frameL, frameL, cv::Size(ksize,ksize), sigma, sigma);
//    cv::Canny(frameL, frameL, 0, 32, 3);
    /////////////////////////////////////////////////////////////////
          //cv::morphologyEx(frameL,frameL,cv::MORPH_DILATE,cv::Mat(),cv::Point(-1,-1),5);
    //cv::HoughLines(frameL,)




    cv::namedWindow( "Cany", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "RAW", CV_WINDOW_AUTOSIZE);
    cv::imshow( "Cany", frameL );
    cv::imshow( "RAW", frameR );
    cv::waitKey(0);

}
