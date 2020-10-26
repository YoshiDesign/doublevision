#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#define DEVICE_ID	0

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


class FrameProcessor {
public:
	virtual void process(cv::Mat &input, cv::Mat &output) = 0;
};

class Camera {

public:

	Camera(){};
	Camera(int device)
	{
	
		device = device;

	}

	void check(const char* check) {
		std::cout << "CHECK!!" << check << std::endl;
	}

	void run(const char *type = "default")
	{
		// In case something was already cap'ing
		cap.release();
		// Acquire the camera feed
		cap.open(device);
		
		// Assign & Call to member function pointer
		// func = &Camera::check;
		// ((*this).*(func))("Lol");

		if (!cap.isOpened())
		{
			std::cerr << "Error opening capture device" << std::endl;
		}

		if (type == "edge") {
			// Perform edge detection when broadcasting
			useProcess = 1;
			setFrameProcessor(detect_edges);
			process(frame, outputFrame);

		
		}
		
		std::cout << "UseProcess: " << useProcess << std::endl;
		// Capture loop
		for (;;)
		{
			if (cv::waitKey(5) >= 0)
				break;

			// For video sequences?
			if (!readNextFrame(frame))
				break;

//			cv::imshow("Live", frame);
			if (useProcess)
			{
				process(frame, outputFrame);
			} else {
				outputFrame = frame;
			}

			cv::imshow("Hello", outputFrame);
		}


	}

	bool readNextFrame(cv::Mat& frame)
	{
		if (images.size() == 0)
		{
			return cap.read(frame);
		}
		else 
		{
			std::cerr << "No Images to apply to frame" << std::endl;
			return false;
		}
	}



	void setFrameProcessor(void(*frameProCallback)(cv::Mat&, cv::Mat&))
	{
		frameproc = 0;
		process = frameProCallback;
	}

private:
	cv::Mat frame;
	cv::Mat outputFrame;
	cv::VideoCapture cap;
	
	FrameProcessor *frameproc;

	int device;
	int useProcess = 0;
	
	/* A lesson in function ptrs:
	 * @process is a function ptr to an external function - readable
	 * @func is a function ptr to a member function - poor readability, check out its call
	 *
	 * Class F'n pointers which point to their own members fn's
	 * are probably best avoided unless absolutely necessary
	 */
	void (*process)(cv::Mat&, cv::Mat&);
	// void (Camera::*func)(const char*);

	std::vector<std::string> images;

};


int main(int argc, char** argv )
{

//	cv::Mat frame;
//	cv::VideoCapture cap;

//	int apiID = cv::CAP_ANY;

//	cap.open(DEVICE_ID + apiID);

	Camera camera(DEVICE_ID + cv::CAP_ANY);
	camera.run("edge");	

	return 0;
	
}
