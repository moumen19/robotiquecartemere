#include "Camera.hpp"

//ctor
Camera::Camera(int cameraIndex):cv::VideoCapture(cameraIndex)
{

}

Camera::Camera(const string& filename):cv::VideoCapture(filename)
{

}

//dtor
Camera::~Camera()
{

}
