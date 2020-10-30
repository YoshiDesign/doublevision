#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include "visionlib.hpp"

#define LOG(m) std::cout << m << std::endl;

namespace vis { namespace camera {

/*
 * A frame processor function - Edge detection
 */ 
void detect_edges(cv::Mat& img, cv::Mat& out)
{
	if (img.channels() == 3)
	{
		cv::cvtColor(img, out, cv::COLOR_BGR2GRAY);
	}
	// compute edges
	cv::Canny(out, out, 100, 200);
	// invert image binary
	cv::threshold(out, out, 128, 255, cv::THRESH_BINARY_INV);
}

Camera::Camera(){
	this->device = 0;
	this->useProcess = 0;
		
}	
Camera::Camera(int device): device(device){}

void Camera::run(const char *type = "default")
{	
	cv::Mat frame;
	cv::Mat outputFrame;

	// In case something was already cap'ing
	cap.release();
	// Acquire the camera feed
	cap.open(0);
	
	if (!cap.isOpened())
	{
		std::cerr << "Error opening capture device" << std::endl;
	}

	if (strcmp(type, "edge")==0) {
		// Perform edge detection when broadcasting
		useProcess = 1;
		setFrameProcessor(detect_edges);
		process(frame, outputFrame);
	}

	std::cout << "Processing frames..." << std::endl;	

	// Capture loop
	for (;;)
	{
		if (cv::waitKey(5) >= 0)
			break;

		// For video sequences
		if (!readNextFrame(frame))
			break;

		if (useProcess)
		{
			std::cout << "Processing Frame" << std::endl;
			process(frame, outputFrame);
		} else {
			std::cout << "Non-processed Frame" << std::endl;
			outputFrame = frame;
		}

		cv::imshow("Hello", outputFrame);
	}

}

/*
 * For video - Detect when we've run out of frames
 */ 
bool Camera::readNextFrame(cv::Mat& frame)
{
	if (images.size() == 0)
	{
		std::cout << "Cap Frames" << std::endl;
		return cap.read(frame);

	} else {
		std::cerr << "No Images to apply to frame" << std::endl;
		return false;
	}	
}

/*
 * Set the callback function to be executed when we call process()
 */ 
void Camera::setFrameProcessor(void(*frameProCallback)(cv::Mat&, cv::Mat&))
{
	//frameproc = 0;
	process = frameProCallback;
}


} // --Namespace camera 
} // --Namespace vis
