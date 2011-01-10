#include "Camera.hpp"

#include <sstream>


//ctor
Camera::Camera(int index):m_index(index),m_Capture(NULL)
{

}

//dtor
Camera::~Camera()
{
    close();
}

void Camera::open()
{
    // creates Capture from camera
    m_Capture = cvCreateCameraCapture( m_index );

    // security check
    if ( !m_Capture ) {
        fprintf( stderr, "Cannot open initialize webcam!\n" );
        exit(1);
    }
    /*
    // To Do:
    //
    // with exception, try 'm_Capture' => if false, error to throw
    */

    // fill in a index, height, width as Camera informations
    std::ostringstream oss;
    oss << "Camera " << m_index;
    oss	<< " (" << cvGetCaptureProperty( m_Capture, CV_CAP_PROP_FRAME_WIDTH );
    oss	<< "x"  << cvGetCaptureProperty( m_Capture, CV_CAP_PROP_FRAME_HEIGHT ) << ")";
    m_infos = oss.str();
}

void Camera::close()
{
    if(m_Capture)
        cvReleaseCapture(&m_Capture);

    m_Capture = NULL;
}

cv::Mat Camera::getFrame()
{
    return cvQueryFrame( m_Capture );
}
