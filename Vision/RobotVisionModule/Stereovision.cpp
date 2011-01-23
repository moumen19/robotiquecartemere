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
            m_LeftCamera = new Camera("//host//TRAVAIL//M2//Vision//Echantillons//stereo_videos//ground1//Vid_Left_0.avi");
            if(!m_LeftCamera->isOpened()){
                cerr<<"Fail to open the left video !"<<endl;
                throw (new std::exception());
            }
            // right
            m_RightCamera = new Camera("//host//TRAVAIL//M2//Vision//Echantillons//stereo_videos//ground1//Vid_Right_0.avi");
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


// display both cameras untouched (same as LiveDisplay in class Camera)
void Stereovision::RawDisplay()
{
    char keyPressed = -1;
    int videoOutputCount = 0;
    int imageOutputCount = 0;
    bool videoBeingRecorded = false;
    cv::VideoWriter videoWriterL, videoWriterR;
    cv::Mat frameL, frameR;
    cv::namedWindow( "rawDisplay_Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "rawDisplay_Right", CV_WINDOW_AUTOSIZE);

    while(true)
    {
        // Exit loop
        if(keyPressed == STOP_DISPLAY_KEY) break;

        // left
        *m_LeftCamera >> frameL;
        if(frameL.empty()) break;

        // right
        *m_RightCamera >> frameR;
        if(frameR.empty())break;

        // Image capture : 'i'
        if(keyPressed == 105){
            stringstream convert;
            convert << imageOutputCount++ ;
            imwrite(string("Images/Img_left_") + convert.str() + string(".jpg"), frameL); // Writes the image
            imwrite(string("Images/Img_right_") + convert.str() + string(".jpg"), frameR); // Writes the image
        }
        // Video capture : 'v'
        if(keyPressed == VIDEO_RECORD_KEY){
                if(!videoBeingRecorded){
                    videoBeingRecorded = true; // starts recording

                    stringstream convert;
                    convert << videoOutputCount++ ;
                    videoWriterL.open(string("Videos/Vid_Left_")+ convert.str() +string(".avi"), CV_FOURCC('M', 'J', 'P', 'G'), 20, frameL.size()); // Opens the video
                    videoWriterR.open(string("Videos/Vid_Right_")+ convert.str() +string(".avi"), CV_FOURCC('M', 'J', 'P', 'G'), 20, frameR.size());
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

         // Get key presses value
        keyPressed = cv::waitKey(WAITING_TIME_MS);
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

void Stereovision::FloodFilling()
{
    cv::Mat image0 = cv::imread("//host//TRAVAIL//M2//Vision//Echantillons//stereo_images//Img_right_4.jpg",1);

    cv::Mat image, gray, mask;
    int ffillMode = 1;
    int loDiff = 20, upDiff = 20;
    int connectivity = 4;
    int isColor = true;
    bool useMask = true;
    int newMaskVal = 255;


    image0.copyTo(image);
    cv::cvtColor(image0, gray, CV_BGR2GRAY);


    cv::Point seed = cv::Point(300,350);
    int lo = ffillMode == 0 ? 0 : loDiff;
    int up = ffillMode == 0 ? 0 : upDiff;
    int flags = connectivity + (newMaskVal << 8) +
                (ffillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);
    int b = (unsigned)cv::theRNG() & 255;
    int g = (unsigned)cv::theRNG() & 255;
    int r = (unsigned)cv::theRNG() & 255;
    cv::Rect ccomp;

    cv::Scalar newVal = isColor ? cv::Scalar(b, g, r) : cv::Scalar(r*0.299 + g*0.587 + b*0.114);
    cv::Mat dst = isColor ? image : gray;
    int area;

    //mask
    mask.create(image0.rows+2, image0.cols+2, CV_8UC1);

    cv::rectangle(mask, cv::Point(200,100), cv::Point(300,300), cv::Scalar(b, g, r));
    cv::rectangle(mask, cv::Point(350,100), cv::Point(500,300), cv::Scalar(b, g, r));

    cv::imshow( "mask", mask );
    cv::waitKey(0);

    if( useMask )
    {
        //cv::threshold(mask, mask, 1, 128, CV_THRESH_BINARY);


        area = cv::floodFill(dst, mask, seed, newVal, &ccomp, cv::Scalar(lo, lo, lo),
                  cv::Scalar(up, up, up), flags);
        cv::imshow( "mask", mask );
    }
    else
    {
        area = cv::floodFill(dst, seed, newVal, &ccomp, cv::Scalar(lo, lo, lo),
                  cv::Scalar(up, up, up), flags);
    }



    cv::namedWindow( "image", 1 );
    cv::imshow("image", isColor ? image : gray);


    cv::imshow("image", dst);
    cout << area << " pixels were repainted\n";

        cv::waitKey(0);

}

// main routine of the video processing module
void Stereovision::Run()
{
    cv::Mat frameL, frameR;
    cv::namedWindow( "Canny Left", CV_WINDOW_AUTOSIZE);
    cv::namedWindow( "Canny Right", CV_WINDOW_AUTOSIZE);

    while(cv::waitKey(100) < 1)
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
//        cv::morphologyEx(frameL,frameL,cv::MORPH_CLOSE,cv::Mat(2,3,CV_8U),cv::Point(-1,-1), 2);
//        cv::morphologyEx(frameR,frameR,cv::MORPH_OPEN,cv::Mat(3,3,CV_8U),cv::Point(-1,-1), 2);
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


    cv::Mat frameL= cv::imread("//host//TRAVAIL//M2//Vision//Echantillons//stereo_images//Img_right_4.jpg",1);
    cv::Mat frameR= cv::imread("//host//TRAVAIL//M2//Vision//Echantillons//stereo_images//Img_left_4.jpg",1);

    m_LeftImageBuffer.push_back(frameL);
    m_RightImageBuffer.push_back(frameR);

    // FloodFill ( 55 / 22 )

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
