#include <vector>
#include <sstream>
#include <string>
#include <unistd.h>
#include "visionlib.hpp"

#define LOG(m) std::cout << m << std::endl
#define ELOG(m) std::cerr << m << std::endl

namespace vis { namespace camera {


void pixelProcess(cv::Mat_<cv::Vec3b>& img){

	int r,g,b;
	int div = img.rows * img.cols;
	r = g = b = 0;


	cv::Vec3b px;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
		
			px = img.at<cv::Vec3b>(i,j);
			r += px[2];
			g += px[1];
			b += px[0];


		}
	}

	int rsub = r / div;
	int gsub = g / div;
	int bsub = b / div;

	cv::MatIterator_<cv::Vec3b> itBegin = img.begin();
	cv::MatIterator_<cv::Vec3b> itEnd = img.end();

	for ( ; itBegin != itEnd ; itBegin++)
	{

		(*itBegin)[0] -= bsub;
		(*itBegin)[1] -= gsub;
		(*itBegin)[2] -= rsub;

		if ((*itBegin)[0] < 0) (*itBegin)[0] = 255;
		if ((*itBegin)[1] < 0) (*itBegin)[1] = 255;
		if ((*itBegin)[2] < 0) (*itBegin)[2] = 255;

	}

//	std::cout << "Snapshot:\n" << "R:\t" << r/div << "\nG:\t" << g/div << "\nB:\t" << b/div << std::endl;
		
}

/*
 * A frame processor function - Edge detection
 */ 
void detect_edges(cv::Mat_<cv::Vec3b>& img, cv::Mat_<cv::Vec3b>& out)
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
Camera::Camera(int device): device{device}{}

void Camera::run(std::queue<int>& Q, const char *type = "default")
{	
	// Original frame
	cv::Mat_<cv::Vec3b> frame;
	// Used as processed frame
	cv::Mat_<cv::Vec3b> outputFrame;

	bool toggleProc = false;
	
	// Acquire the camera resource 
	cap.release();
	cap.open(0);
	
	if (!cap.isOpened())
	{
		ELOG("Error opening Capture device.");
	}

	if (strcmp(type, "edge") == 0) {
		// Perform edge detection when broadcasting
		useProcess = 1;
		setFrameProcessor(detect_edges);
	}

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
			process(frame, outputFrame);
		} else {
			outputFrame = frame;
		}

		// Send the current output frame
		if(!Q.empty())
		{
			// Remove the signal indication
			 nt val = Q.front();
			Q.pop();
			switch(val) {
				/**
				 * Effects of each gpio pin reading
				 */ 
			
				case 17:
					// Snapshot
					pixelProcess(frame);
					break;
				case  18:
					// Toggle filter layer
					toggleProc = !toggleProc;
					break;
			}
		}

		if (toggleProc)
			pixelProcess(frame);

		if(!outputFrame.empty())
			cv::imshow("Hello", frame);
	}

}

/*
 * For video - Detect when we've run out of frames
 */ 
bool Camera::readNextFrame(cv::Mat_<cv::Vec3b>& frame)
{
	if (images.size() == 0)
	{
		return cap.read(frame);

	} else {
		LOG("End of frame sequence");
		return false;
	}	
}

/*
 * Set the callback function to be executed when we call process()
 */ 
void Camera::setFrameProcessor(void(*frameProCallback)(cv::Mat_<cv::Vec3b>&, cv::Mat_<cv::Vec3b>&))
{
	//frameproc = 0;
	process = frameProCallback;
}


} // --Namespace camera 
} // --Namespace vis
