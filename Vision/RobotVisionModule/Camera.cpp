#include "Camera.hpp"

//constructors
Camera::Camera():cv::VideoCapture(0), m_Name(string("defaultCam"))
{

}

Camera::Camera(int cameraIndex):cv::VideoCapture(cameraIndex),m_Name(string("Cam_"))
{
    stringstream convert; // stringstream used for the conversion
    convert << cameraIndex; //add the value of cameraIndex to the characters in the stream
    m_Name += convert.str(); // m_Name now has the good value
}

Camera::Camera(const string& filename):cv::VideoCapture(filename),m_Name(filename)
{

}

//destructor
Camera::~Camera()
{

}

// Allows also
void Camera::LiveDisplay()
{
    if(this->isOpened()){

        char keyPressed = -1;
        int imageOutputCount = 0;
        int videoOutputCount = 0;
        cv::VideoWriter videoWriter;
        cv::Mat Frame;
        bool videoBeingRecorded = false;

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
                stringstream convert;
                convert << imageOutputCount++ ;
                imwrite(string("Images/Img_") + convert.str() + string(".jpg"), Frame); // Writes the image
            }

            // Video Capture
            if(keyPressed == VIDEO_RECORD_KEY){
                if(!videoBeingRecorded){
                    videoBeingRecorded = true; // starts recording

                    // opens file for writer
                    stringstream convert;
                    convert << videoOutputCount++ ;
                    videoWriter.open(string("Videos/Vid_")+ convert.str() +string(".avi"), CV_FOURCC('M', 'J', 'P', 'G'), RECORDING_FPS, Frame.size()); // Opens the video

                    if(!videoWriter.isOpened()) { cerr<<"Error trying to open the video writer!" <<endl; break;}
                }
                else{
                    videoBeingRecorded = false; // stops recording
                }
            }
            if(videoBeingRecorded){ // records
               videoWriter << Frame;
               cout<< "recording video stream..." <<endl;
            }

            // Display frame
            cv::imshow(string("Display ")+ m_Name, Frame );
        }
    }
    else{
        cerr<<"The camera is not opened !"<<endl;
    }
}

// to be improved
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
	CvSize imagesize = cvSize( board_w, board_h );

	// CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	cv::Mat capture;

	//cvSetCaptureProperty( capture, CV_CAP_PROP_MODE, MODE_640x480_YUV411 );
	//assert( capture );

	cv::namedWindow("Calibration", CV_WINDOW_AUTOSIZE);
	// Allocate Sotrage
	CvMat* image_points		= cvCreateMat( n_boards*board_n, 2, CV_32FC1 );
	CvMat* chessboardplanCoordinates   = cvCreateMat( n_boards*board_n, 3, CV_32FC1 );
	CvMat* point_counts		= cvCreateMat( n_boards, 1, CV_32SC1 );
	CvMat* intrinsic_matrix		= cvCreateMat( 3, 3, CV_32FC1 );
	CvMat* distortion_coeffs	= cvCreateMat( 4, 1, CV_32FC1 );

	CvPoint2D32f* corners = new CvPoint2D32f[ board_n ];
	int corner_count;
	int successes = 0;
	int step, frame = 0;

    IplImage * image;

	std::cout << "Started calibration of Intrinsic parameters" << std::endl;

    while( successes < n_boards ){

    *this>>capture;

    IplImage tempcapture = capture;
    image = & tempcapture;

//	IplImage *image = cvQueryFrame( &cvcapture );
	IplImage *gray_image = cvCreateImage( cvGetSize( image ), 8, 1 );

	// Capture Corner views loop until we've got n_boards
	// succesful captures (all corners on the board are found)


		// Skip every board_dt frames to allow moving the chessboard
		if( frame++ % board_dt == 0 ){
			// Find chessboard corners:
			int found = cvFindChessboardCorners( image, imagesize, corners,
				&corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS );

			// Get subpixel accuracy on those corners
			cvCvtColor( image, gray_image, CV_BGR2GRAY );
			cvFindCornerSubPix( gray_image, corners, corner_count, cvSize( 11, 11 ),
				cvSize( -1, -1 ), cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

			// Draw it
			cvDrawChessboardCorners( image, imagesize, corners, corner_count, found );
			cvShowImage( "Calibration", image );

			// If we got a good board, add it to our data
			if( corner_count == board_n ){
				step = successes*board_n;
				for( int i=step, j=0; j < board_n; ++i, ++j ){
					CV_MAT_ELEM( *image_points, float, i, 0 ) = corners[j].x;
					CV_MAT_ELEM( *image_points, float, i, 1 ) = corners[j].y;
					CV_MAT_ELEM( *chessboardplanCoordinates, float, i, 0 ) = j/board_w;
					CV_MAT_ELEM( *chessboardplanCoordinates, float, i, 1 ) = j%board_w;
					CV_MAT_ELEM( *chessboardplanCoordinates, float, i, 2 ) = 0.0f;
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
//		if( c == 'x' || c == 27 );
//			return ;

//	IplImage tempcapture = capture;
//    IplImage * image = & tempcapture; // Get next image

	} // End collection while loop

	// Allocate matrices according to how many chessboards found
	CvMat* object_points2 = cvCreateMat( successes*board_n, 3, CV_32FC1 );
	CvMat* image_points2 = cvCreateMat( successes*board_n, 2, CV_32FC1 );
	CvMat* point_counts2 = cvCreateMat( successes, 1, CV_32SC1 );

	// Transfer the points into the correct size matrices
	for( int i = 0; i < successes*board_n; ++i ){
		CV_MAT_ELEM( *image_points2, float, i, 0) = CV_MAT_ELEM( *image_points, float, i, 0 );
		CV_MAT_ELEM( *image_points2, float, i, 1) = CV_MAT_ELEM( *image_points, float, i, 1 );
		CV_MAT_ELEM( *object_points2, float, i, 0) = CV_MAT_ELEM( *chessboardplanCoordinates, float, i, 0 );
		CV_MAT_ELEM( *object_points2, float, i, 1) = CV_MAT_ELEM( *chessboardplanCoordinates, float, i, 1 );
		CV_MAT_ELEM( *object_points2, float, i, 2) = CV_MAT_ELEM( *chessboardplanCoordinates, float, i, 2 );
	}

	for( int i=0; i < successes; ++i ){
		CV_MAT_ELEM( *point_counts2, int, i, 0 ) = CV_MAT_ELEM( *point_counts, int, i, 0 );
	}
	cvReleaseMat( &chessboardplanCoordinates );
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
    m_chessboardplanCoordinates = chessboardplanCoordinates;
    m_cornersMat = image_points;
    m_nbTotalCorners = point_counts;


    m_image_size.width=  imagesize.width;
    m_image_size.height= imagesize.height;

	std::cout << "Success: calibration from target..." << std::endl;
}

// to be improved
void Camera::CalibrateFromImageSet()
{
//    int i;
//    int ballsPixelsSizes_Left[3] = {0,0,0};
//    int ballsPixelsSizes_Right[3] = {0,0,0};
//    float fx_Left=0,fy_Left=0,fx_Right=0,fy_Right=0;
//    float Z_Left [3][2]= {{0,0},{0,0},{0,0}}, Z_Right [3][2]= {{0,0},{0,0},{0,0}};
//    char imageFileName[200] = "";
//    CvSize imageSize = cvSize(1,1);
//    CvSize chessboardSize = cvSize(nbColumns,nbLines);
//
//    // matrices for corners computation
//    CvPoint2D32f cornersList_Right[nbImages*nbCorners];
//    CvPoint2D32f cornersList_Left[nbImages*nbCorners];
//
//    // matrices for the camera calibration
//    CvMat * intrinsicMatrix_Left  = cvCreateMat(3,3,CV_32FC1);
//    CvMat * intrinsicMatrix_Right  = cvCreateMat(3,3,CV_32FC1);
//    CvMat * distortionCoeffs_Left = cvCreateMat(4,1,CV_32FC1); // only 4 coeffs will be used (5th = 0)
//    CvMat * distortionCoeffs_Right = cvCreateMat(4,1,CV_32FC1);
//    CvMat * nbTotalCorners = cvCreateMat(nbImages,1,CV_32SC1); // array containing the corners count for each picture
//    CvMat * chessboardplanCoordinates = cvCreateMat(nbImages*nbCorners,3,CV_32FC1); // 3D: x, y, z=0
//    CvMat * cornersMat_Left = cvCreateMat(nbImages*nbCorners,2,CV_32FC1);   // 2D: x, y
//    CvMat * cornersMat_Right = cvCreateMat(nbImages*nbCorners,2,CV_32FC1);
//
//    // Chessboard corners computation
//    for(i=0;i<nbImages;i++)
//    {
//        // Left camera
//        sprintf(imageFileName,"calibration//image_calibration/gauche%d.jpg",i+1);
//
//        // this allows to fill in a 1D array for several images, with a function still able to work on individual images
//        compute_and_display_image_corners(imageFileName, &imageSize, chessboardSize, &cornersList_Left[i*nbCorners]);
//
//        // Right camera
//        sprintf(imageFileName,"calibration//image_calibration/droite%d.jpg",i+1);
//
//
//        compute_and_display_image_corners(imageFileName, &imageSize, chessboardSize, &cornersList_Right[i*nbCorners]);
//    }
//
//    // Prepare some matrices
//    initiate_matrices_for_calibration(intrinsicMatrix_Left, distortionCoeffs_Left,
//                                      nbTotalCorners, chessboardplanCoordinates,
//                                      cornersMat_Left, cornersList_Left); // convert list to matrix
//
//    initiate_matrices_for_calibration(intrinsicMatrix_Right, distortionCoeffs_Right,
//                                      nbTotalCorners, chessboardplanCoordinates,
//                                      cornersMat_Right, cornersList_Right);
//
//    // compute intrinsic & distortion parameters
//    cvCalibrateCamera2(chessboardplanCoordinates, cornersMat_Left,
//                        nbTotalCorners, imageSize,
//                        intrinsicMatrix_Left, distortionCoeffs_Left,
//                        NULL, NULL, 0 //CV_CALIB_FIX_ASPECT_RATIO
//                        );
//
//    cvCalibrateCamera2(chessboardplanCoordinates, cornersMat_Right,
//                        nbTotalCorners, imageSize,
//                        intrinsicMatrix_Right, distortionCoeffs_Right,
//                        NULL, NULL, 0 //CV_CALIB_FIX_ASPECT_RATIO
//                        );
//
//    m_intrinsecMatrix_Left = intrinsicMatrix_Left;
//    m_intrinsecMatrix_Right = intrinsicMatrix_Right;
//    m_distortionMatrix_Left = distortionCoeffs_Left;
//    m_distortionMatrix_Right = distortionCoeffs_Right;
//    m_chessboardplanCoordinates = chessboardplanCoordinates;
//    m_cornersMat_Left = cornersMat_Left;
//    m_cornersMat_Right = cornersMat_Right;
//    m_nbTotalCorners = nbTotalCorners;
//
//    m_image_size.width=  imageSize.width;
//    m_image_size.height= imageSize.height;
//
//
//
//    std::cout << "Success: calibration from target..." << std::endl;
//
//
//    cv::FileStorage fs("calibration//Parameters//left_camera.xml", cv::FileStorage::WRITE);
//    if (fs.isOpened())
//    {   fs << "intrinsec" << m_intrinsecMatrix_Left;
//        fs << "distortion" << m_distortionMatrix_Left;
//        fs << "chessboardplanCoordinates" << m_chessboardplanCoordinates;
//        fs << "cornersMat" << m_cornersMat_Left;
//        fs << "nbTotalCorners" << m_nbTotalCorners;
//        fs << "image_size_width" << (int) m_image_size.width;
//        fs << "image_size_height" << (int) m_image_size.height;
//        fs.release();
//    }
//
//	std::cout << "Success: Saved cam_Left..." << std::endl;
//
//	    cv::FileStorage fs2("calibration//Parameters//right_camera.xml", cv::FileStorage::WRITE);
//    if (fs2.isOpened())
//    {   fs2 << "intrinsec" << m_intrinsecMatrix_Right;
//        fs2 << "distortion" << m_distortionMatrix_Right;
//        fs2 << "object_point" << m_chessboardplanCoordinates;
//        fs2 << "cornersMat" << m_cornersMat_Right;
//        fs2.release();
//    }
//
//	std::cout << "Success: Saved cam_Right..." << std::endl;

}

// to be improved
void Camera::SaveMatrix(const string &filename)
{

    cv::FileStorage fs("calibration//Parameters//" + filename +".xml", cv::FileStorage::WRITE);
    if (!fs.isOpened())
    {  fs.open("calibration//Parameters//" + filename  +  ".xml", FileStorage::WRITE);
        fs << "intrinsec" << m_intrinsecMatrix;
        fs << "distortion" << m_distortionMatrix;
        fs << "chessboardplanCoordinates" << m_chessboardplanCoordinates;
        fs << "cornersMat" << m_cornersMat;
        fs << "nbTotalCorners" << m_nbTotalCorners;
        fs << "image_size" << m_image_size;
        fs.release();
    }

	std::cout << "Success: Saved..." << std::endl;
}


