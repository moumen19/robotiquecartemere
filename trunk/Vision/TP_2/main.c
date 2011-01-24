
#include <stdlib.h>
#include <stdio.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
//#include <cvaux.h>
//#include <cxcore.h>


#include "cameraCalibration.h"
#include "colorSegmentation.h"
#include "stereovision.h"

// from colorSegmentation: here just to be released
extern IplImage * imgBGR;
extern IplImage * imgHSV;
extern IplImage * grayMaskHSV;
extern IplImage * improvedMaskOnBGR;

////////////////////////////////////  Method  ////////////////////////////////////////////////////////////////////
// in order to calibrate a camera, we need to know the distortion coefficients and the rotation/translation vetors
// 1/ to identify the calibration object: here a 2D-chessboard pattern => cvFindChessboardCorners ()
// 2/ to be more precise on corners identification => cvFindCornerSubPix ()
// 3/ to visually check corners identification => cvDrawChessboardCorners()
// 4/ to take in acounts the planar homography in the chessboard projection => cvFindHomography() (if necessary)
// 5/ Finally, to use cvCalibrateCamera2() which provides the extra ant intrinsics parameters of the camera
int main(int argc, char *argv[])
{
    int i;
    int ballsPixelsSizes_Left[3] = {0,0,0};
    int ballsPixelsSizes_Right[3] = {0,0,0};
    float fx_Left=0,fy_Left=0,fx_Right=0,fy_Right=0;
    float Z_Left [3][2]= {{0,0},{0,0},{0,0}}, Z_Right [3][2]= {{0,0},{0,0},{0,0}};
    char imageFileName[200] = "";
    CvSize imgSize = cvSize(1,1);
    CvSize chessboardSize = cvSize(nbColumns,nbLines);

    // matrices for corners computation
    CvPoint2D32f cornersList_Right[nbImages*nbCorners];
    CvPoint2D32f cornersList_Left[nbImages*nbCorners];

    // matrices for the camera calibration
    CvMat * intrinsicMatrix_Left  = cvCreateMat(3,3,CV_32FC1);
    CvMat * intrinsicMatrix_Right  = cvCreateMat(3,3,CV_32FC1);
    CvMat * distortionCoeffs_Left = cvCreateMat(4,1,CV_32FC1); // only 4 coeffs will be used (5th = 0)
    CvMat * distortionCoeffs_Right = cvCreateMat(4,1,CV_32FC1);
    CvMat * nbTotalCorners = cvCreateMat(nbImages,1,CV_32SC1); // array containing the corners count for each picture
    CvMat * chessboardplanCoordinates = cvCreateMat(nbImages*nbCorners,3,CV_32FC1); // 3D: x, y, z=0
    CvMat * cornersMat_Left = cvCreateMat(nbImages*nbCorners,2,CV_32FC1);   // 2D: x, y
    CvMat * cornersMat_Right = cvCreateMat(nbImages*nbCorners,2,CV_32FC1);

    // stereovision
    CvMat * RotationMatrix = cvCreateMat(3,3,CV_32FC1); // Matrix
    CvMat * TranslationMatrix = cvCreateMat(3,1,CV_32FC1); // Vector
    CvMat * EssentialMatrix = cvCreateMat(3,3,CV_32FC1);
    CvMat * FundamentalMatrix = cvCreateMat(3,3,CV_32FC1);
    CvPoint light_Left = cvPoint(0,0);
    CvPoint light_Right = cvPoint(0,0);

    // Chessboard corners computation
    for(i=0;i<nbImages;i++)
    {
        // Left camera
        sprintf(imageFileName,"Calibration_TP2/gauche%d.jpg",i+1);

        // this allows to fill in a 1D array for several images, with a function still able to work on individual images
        compute_and_display_image_corners(imageFileName, &imgSize, chessboardSize, &cornersList_Left[i*nbCorners]);

        // Right camera
        sprintf(imageFileName,"Calibration_TP2/droite%d.jpg",i+1);


        compute_and_display_image_corners(imageFileName, &imgSize, chessboardSize, &cornersList_Right[i*nbCorners]);
    }

    // Prepare some matrices
    initiate_matrices_for_calibration(intrinsicMatrix_Left, distortionCoeffs_Left,
                                      nbTotalCorners, chessboardplanCoordinates,
                                      cornersMat_Left, cornersList_Left); // convert list to matrix

    initiate_matrices_for_calibration(intrinsicMatrix_Right, distortionCoeffs_Right,
                                      nbTotalCorners, chessboardplanCoordinates,
                                      cornersMat_Right, cornersList_Right);

    // compute intrinsic & distortion parameters
    cvCalibrateCamera2(chessboardplanCoordinates, cornersMat_Left,
                        nbTotalCorners, imgSize,
                        intrinsicMatrix_Left, distortionCoeffs_Left,
                        NULL, NULL, 0 //CV_CALIB_FIX_ASPECT_RATIO
                        );

    cvCalibrateCamera2(chessboardplanCoordinates, cornersMat_Right,
                        nbTotalCorners, imgSize,
                        intrinsicMatrix_Right, distortionCoeffs_Right,
                        NULL, NULL, 0 //CV_CALIB_FIX_ASPECT_RATIO
                        );

    fx_Left = cvmGet(intrinsicMatrix_Left,0,0);
    fy_Left = cvmGet(intrinsicMatrix_Left,1,1);
    fx_Right = cvmGet(intrinsicMatrix_Right,0,0);
    fy_Right = cvmGet(intrinsicMatrix_Right,1,1);

    // save parameters into files
    cvSave("Calibration_TP2//Parameters//Intrinsic_Left_Cam.xml", intrinsicMatrix_Left,0,0,cvAttrList(NULL,NULL));
    cvSave("Calibration_TP2//Parameters//Distortion_Left_Cam.xml", distortionCoeffs_Left,0,0,cvAttrList(NULL,NULL));
    cvSave("Calibration_TP2//Parameters//Intrinsic_Right_Cam.xml", intrinsicMatrix_Right,0,0,cvAttrList(NULL,NULL));
    cvSave("Calibration_TP2//Parameters//Distortion_Right_Cam.xml", distortionCoeffs_Right,0,0,cvAttrList(NULL,NULL));

    // Check parameters
//    get_balls_pixelsSizes(ballsPixelsSizes_Left, "Images_TP2//SIER_balle_ga.jpg");
//    get_balls_pixelsSizes(ballsPixelsSizes_Right, "Images_TP2//SIER_balle_dr.jpg");

    ///////////////////
    // Z = D/d*f /10 //
    ///////////////////
    // Z: camera-ball distance
    // f: focal
    // d: apparent ball size (pixels)
    // D: real ball size (cm)
    // 10: mm -> cm
    Z_Left[blue][0] = ((float)D / (float)ballsPixelsSizes_Left[blue]) * fx_Left /10;
    Z_Left[blue][1] = ((float)D / (float)ballsPixelsSizes_Left[blue]) * fy_Left /10;
    Z_Left[green][0] = ((float)D / (float)ballsPixelsSizes_Left[green]) * fx_Left /10;
    Z_Left[green][1] = ((float)D / (float)ballsPixelsSizes_Left[green]) * fy_Left /10;
    Z_Left[red][0] = ((float)D / (float)ballsPixelsSizes_Left[red]) * fx_Left /10;
    Z_Left[red][1] = ((float)D / (float)ballsPixelsSizes_Left[red]) * fy_Left /10;

    Z_Right[blue][0] = ((float)D / (float)ballsPixelsSizes_Right[blue]) * fx_Right /10;
    Z_Right[blue][1] = ((float)D / (float)ballsPixelsSizes_Right[blue]) * fy_Right /10;
    Z_Right[green][0] = ((float)D / (float)ballsPixelsSizes_Right[green]) * fx_Right /10;
    Z_Right[green][1] = ((float)D / (float)ballsPixelsSizes_Right[green]) * fy_Right /10;
    Z_Right[red][0] = ((float)D / (float)ballsPixelsSizes_Right[red]) * fx_Right /10;
    Z_Right[red][1] = ((float)D / (float)ballsPixelsSizes_Right[red]) * fy_Right /10;

    // Display Z
    printf("\n\tBlue ball\n");
    printf("Left: (fx)%.1fcm  (fy)%.1fcm\n",Z_Left[blue][0],Z_Left[blue][1]);
    printf("Right: (fx)%.1fcm  (fy)%.1fcm\n",Z_Right[blue][0],Z_Right[blue][1]);
    printf("\n\tRed ball\n");
    printf("Left: (fx)%.1fcm  (fy)%.1fcm\n",Z_Left[red][0],Z_Left[red][1]);
    printf("Right: (fx)%.1fcm  (fy)%.1fcm\n",Z_Right[red][0],Z_Right[red][1]);
    printf("\n\tGreen ball\n");
    printf("Left: (fx)%.1fcm  (fy)%.1fcm\n",Z_Left[green][0],Z_Left[green][1]);
    printf("Right: (fx)%.1fcm  (fy)%.1fcm\n",Z_Right[green][0],Z_Right[green][1]);


    // Stereovision
    cvStereoCalibrate (chessboardplanCoordinates,
    cornersMat_Left ,
    cornersMat_Right,
    nbTotalCorners,
    intrinsicMatrix_Left,
    distortionCoeffs_Left,
    intrinsicMatrix_Left,
    distortionCoeffs_Right,
    imgSize,
    RotationMatrix,
    TranslationMatrix,
    EssentialMatrix,
    FundamentalMatrix,

    //cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 100, 1e-5), // default
    cvTermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 30, 1e-6),

    //CV_CALIB_FIX_INTRINSIC //can be used if previous cameras calibration was accurate (as in this example)
    CV_CALIB_USE_INTRINSIC_GUESS // can be used to refine previous intrinsic/distortion parameters
    );

    // save parameters into files
    cvSave("Calibration_TP2//Parameters//Rotation.xml", RotationMatrix,0,0,cvAttrList(NULL,NULL));
    cvSave("Calibration_TP2//Parameters//Translation.xml", TranslationMatrix,0,0,cvAttrList(NULL,NULL));
    cvSave("Calibration_TP2//Parameters//Essential.xml", EssentialMatrix,0,0,cvAttrList(NULL,NULL));
    cvSave("Calibration_TP2//Parameters//Fundamental.xml", FundamentalMatrix,0,0,cvAttrList(NULL,NULL));

    // Check parameters: get points with mouse cursor
    light_Left = get_one_pixel ("Images_TP2/SIER_gauche.jpg");
    light_Right = get_one_pixel ("Images_TP2/SIER_droite.jpg");
    cvStereoTriangulation(light_Left,light_Right,0); // 1 to display

    // End
    cvReleaseMat(&chessboardplanCoordinates);
    cvReleaseMat(&cornersMat_Right);
    cvReleaseMat(&cornersMat_Left);
    cvReleaseMat(&nbTotalCorners);
    cvReleaseMat(&intrinsicMatrix_Left);
    cvReleaseMat(&intrinsicMatrix_Right);
    cvReleaseMat(&distortionCoeffs_Left);
    cvReleaseMat(&distortionCoeffs_Right);
    cvReleaseMat(&RotationMatrix);
    cvReleaseMat(&TranslationMatrix);
    cvReleaseMat(&EssentialMatrix);
    cvReleaseMat(&FundamentalMatrix);
    // colorSegmentation global variables
    cvDestroyAllWindows();
    cvReleaseImage(&imgBGR );
    cvReleaseImage(&imgHSV );
    cvReleaseImage(&grayMaskHSV );
    cvReleaseImage(&improvedMaskOnBGR );
    system("pause");
    return 0;
}
