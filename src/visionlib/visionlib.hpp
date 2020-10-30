#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

namespace vis { namespace camera {

	void detect_edges(cv::Mat& img, cv::Mat& out);

	/*
	 *
	 */
	class Camera {
	public:
		Camera();
		Camera(int device);
		void run(const char* type);
		bool readNextFrame(cv::Mat& frame);

		/*
		 * Assigns a pointer to a callback fn
		 * (Mat& in, Mat& out)
		*/ 
		void setFrameProcessor(void(*frameProCallback)(cv::Mat&, cv::Mat&));

	private:
		int device, useProcess;
//		cv::Mat frame;
//		cv::Mat outputFrame;
		cv::VideoCapture cap;
		void (*process)(cv::Mat&, cv::Mat&) = nullptr;
		std::vector<std::string> images;
	};

} /* -- namespace camera*/ 

namespace image {

	class Image{
	public:
		Image();
		Image(cv::Mat& img);
	private:
		cv::Mat image;
	};

} // -- namespace image
} // -- namespace vis
