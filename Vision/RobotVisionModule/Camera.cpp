#include "Camera.hpp"

//constructors
Camera::Camera():cv::VideoCapture(0), m_Name(string("defaultCam")),
m_ImageCaptureActivated(false),m_VideoCaptureActivated(false)
{

}

Camera::Camera(int cameraIndex):cv::VideoCapture(cameraIndex),m_Name(string("Cam_")),
m_ImageCaptureActivated(false),m_VideoCaptureActivated(false)
{
    stringstream convert; // stringstream used for the conversion
    convert << cameraIndex; //add the value of cameraIndex to the characters in the stream
    m_Name += convert.str(); // m_Name now has the good value
}

Camera::Camera(const string& filename):cv::VideoCapture(filename),m_Name(filename),
m_ImageCaptureActivated(false),m_VideoCaptureActivated(false)
{

}



//destructor
Camera::~Camera()
{

}



void Camera::LiveDisplay()
{
    if(this->isOpened()){

        char keyPressed = -1;
        int imageOutputCount = 0;
        int videoOutputCount = 0;
        bool videoBeingRecorded = false;
        cv::VideoWriter videoWriter;
        cv::Mat Frame;

        // Displaying window
        cv::namedWindow(string("Display ")+ m_Name, CV_WINDOW_AUTOSIZE);

        while(true)
        {
            // grab a frame
            *this >> Frame;

            // checks
            if(Frame.empty() || !this->isOpened()) break;

            // Get key presses value
            keyPressed = cv::waitKey(WAITING_TIME_MS);

            // Exit loop
            if(keyPressed == STOP_DISPLAY_KEY) break;

            // Image Capture
            if(keyPressed == IMAGE_RECORD_KEY){

                if(m_ImageCaptureActivated){
                    stringstream convert;
                    convert << imageOutputCount++ ;

                    imwrite(string("Img_") + convert.str() + string(".jpg"), Frame); // Writes the image
                }
                else{
                    cout<<"The capture mode need to be activated before recording !";
                }
            }

            // Video Capture
            if(keyPressed == VIDEO_RECORD_KEY){
                if(m_VideoCaptureActivated){

                    if(!videoBeingRecorded){
                        videoBeingRecorded = true; // starts recording

                        stringstream convert;
                        convert << videoOutputCount++ ;
                        videoWriter.open(string("Vid_")+ convert.str() +string(".avi"), CV_FOURCC('M', 'J', 'P', 'G'), RECORDING_FPS, Frame.size()); // Opens the video

                        if(!videoWriter.isOpened()) { cerr<<"Error trying to open the video writer!" <<endl; break;}
                    }
                    else{
                        videoBeingRecorded = false; // stops recording
                    }
                }
                else{
                    cout<<"The capture mode need to be activated before recording !";
                }
            }
            if(videoBeingRecorded){
               videoWriter << Frame;
               cout<< "recording" <<endl;
            }


            // Display frame
            cv::imshow(string("Display ")+ m_Name, Frame );
        }
    }
    else{
        cerr<<"Camera is not opened !"<<endl;
    }
}



// Activates capture modes
void Camera::SetImageCapture(bool activated)
{
    m_ImageCaptureActivated = activated;
}
void Camera::SetVideoCapture(bool activated)
{
    m_VideoCaptureActivated = activated;
}
