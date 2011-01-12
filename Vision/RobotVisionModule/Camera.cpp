#include "Camera.hpp"

//ctor
Camera::Camera(unsigned int index):cv::VideoCapture(index)
{

}

//dtor
Camera::~Camera()
{

}
