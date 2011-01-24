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



void Camera::CalibrateFromCamera()
{

    int n_boards = 0;
    const int board_dt = 20;
    int board_w;
    int board_h;


    board_w = 6; // Board width in squares
	board_h = 8; // Board height
	n_boards = 15; // Number of boards
	int board_n = board_w * board_h;
	CvSize board_sz = cvSize( board_w, board_h );

	// CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	CvCapture* capture = cvCreateCameraCapture( 0 );
	//cvSetCaptureProperty( capture, CV_CAP_PROP_MODE, MODE_640x480_YUV411 );
	assert( capture );

	cvNamedWindow( "Calibration" );
	// Allocate Sotrage
	CvMat* image_points		= cvCreateMat( n_boards*board_n, 2, CV_32FC1 );
	CvMat* object_points		= cvCreateMat( n_boards*board_n, 3, CV_32FC1 );
	CvMat* point_counts		= cvCreateMat( n_boards, 1, CV_32SC1 );
	CvMat* intrinsic_matrix		= cvCreateMat( 3, 3, CV_32FC1 );
	CvMat* distortion_coeffs	= cvCreateMat( 4, 1, CV_32FC1 );

	CvPoint2D32f* corners = new CvPoint2D32f[ board_n ];
	int corner_count;
	int successes = 0;
	int step, frame = 0;

	std::cout << "Started calibration of Intrinsic parameters" << std::endl;

	IplImage *image = cvQueryFrame( capture );
	IplImage *gray_image = cvCreateImage( cvGetSize( image ), 8, 1 );

	// Capture Corner views loop until we've got n_boards
	// succesful captures (all corners on the board are found)

	while( successes < n_boards ){
		// Skip every board_dt frames to allow moving the chessboard
		if( frame++ % board_dt == 0 ){
			// Find chessboard corners:
			int found = cvFindChessboardCorners( image, board_sz, corners,
				&corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS );

			// Get subpixel accuracy on those corners
			cvCvtColor( image, gray_image, CV_BGR2GRAY );
			cvFindCornerSubPix( gray_image, corners, corner_count, cvSize( 11, 11 ),
				cvSize( -1, -1 ), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

			// Draw it
			cvDrawChessboardCorners( image, board_sz, corners, corner_count, found );
			cvShowImage( "Calibration", image );

			// If we got a good board, add it to our data
			if( corner_count == board_n ){
				step = successes*board_n;
				for( int i=step, j=0; j < board_n; ++i, ++j ){
					CV_MAT_ELEM( *image_points, float, i, 0 ) = corners[j].x;
					CV_MAT_ELEM( *image_points, float, i, 1 ) = corners[j].y;
					CV_MAT_ELEM( *object_points, float, i, 0 ) = j/board_w;
					CV_MAT_ELEM( *object_points, float, i, 1 ) = j%board_w;
					CV_MAT_ELEM( *object_points, float, i, 2 ) = 0.0f;
				}
				CV_MAT_ELEM( *point_counts, int, successes, 0 ) = board_n;
				successes++;
			}
		}

		// Handle pause/unpause and ESC
		int c = cvWaitKey( 15 );
		if( c == 'p' ){
			c = 0;
			while( c != 'p' && c != 27 && c!= 'x' ){
				c = cvWaitKey( 250 );
			}
		}
		if( c == 'x' || c == 27 );
			return ;

		image = cvQueryFrame( capture ); // Get next image
	} // End collection while loop

	// Allocate matrices according to how many chessboards found
	CvMat* object_points2 = cvCreateMat( successes*board_n, 3, CV_32FC1 );
	CvMat* image_points2 = cvCreateMat( successes*board_n, 2, CV_32FC1 );
	CvMat* point_counts2 = cvCreateMat( successes, 1, CV_32SC1 );

	// Transfer the points into the correct size matrices
	for( int i = 0; i < successes*board_n; ++i ){
		CV_MAT_ELEM( *image_points2, float, i, 0) = CV_MAT_ELEM( *image_points, float, i, 0 );
		CV_MAT_ELEM( *image_points2, float, i, 1) = CV_MAT_ELEM( *image_points, float, i, 1 );
		CV_MAT_ELEM( *object_points2, float, i, 0) = CV_MAT_ELEM( *object_points, float, i, 0 );
		CV_MAT_ELEM( *object_points2, float, i, 1) = CV_MAT_ELEM( *object_points, float, i, 1 );
		CV_MAT_ELEM( *object_points2, float, i, 2) = CV_MAT_ELEM( *object_points, float, i, 2 );
	}

	for( int i=0; i < successes; ++i ){
		CV_MAT_ELEM( *point_counts2, int, i, 0 ) = CV_MAT_ELEM( *point_counts, int, i, 0 );
	}
	cvReleaseMat( &object_points );
	cvReleaseMat( &image_points );
	cvReleaseMat( &point_counts );

	// At this point we have all the chessboard corners we need
	// Initialize the intrinsic matrix such that the two focal lengths
	// have a ratio of 1.0

	CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0;
	CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0;

	// Calibrate the camera
	cvCalibrateCamera2( object_points2, image_points2, point_counts2, cvGetSize( image ),
		intrinsic_matrix, distortion_coeffs, NULL, NULL, CV_CALIB_FIX_ASPECT_RATIO );

    m_intrinsecMatrix = intrinsic_matrix;
    m_distortionMatrix = distortion_coeffs;

	std::cout << "Success: calibration from target..." << std::endl;


}

void Camera::SaveMatrix(const string &filename)
{

    cv::FileStorage intrinsec("Parameters\\Intrinsecs" + filename  +  ".xml", cv::FileStorage::WRITE);

    intrinsec << m_intrinsecMatrix;

    cv::FileStorage distortion("Parameters\\Distortion" + filename  +  ".xml", cv::FileStorage::WRITE);

    distortion << m_distortionMatrix;

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

                    imwrite(string("Images/Img_") + convert.str() + string(".jpg"), Frame); // Writes the image
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
                        videoWriter.open(string("Videos/Vid_")+ convert.str() +string(".avi"), CV_FOURCC('M', 'J', 'P', 'G'), RECORDING_FPS, Frame.size()); // Opens the video

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
        cerr<<"The camera is not opened !"<<endl;
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
