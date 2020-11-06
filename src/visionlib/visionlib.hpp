#pragma once
#include <iostream>
#include <queue>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>


namespace vis { namespace camera {

	void vgg16(cv::Mat_<cv::Vec3b>& img);
	void detect_edges(cv::Mat_<cv::Vec3b>& img, cv::Mat_<cv::Vec3b>& out);

	/*
	 *
	 */
	class Camera {
	public:
		Camera();
		Camera(int device);
		void run(std::queue<int>& Q, const char* type);
		bool readNextFrame(cv::Mat_<cv::Vec3b>& frame);

		/*
		 * Assigns a pointer to a callback fn
		 * (Mat& in, Mat& out)
		*/ 
		void setFrameProcessor(void(*frameProCallback)(cv::Mat_<cv::Vec3b>&, cv::Mat_<cv::Vec3b>&));

	private:
		int device, useProcess;
//		cv::Mat outputFrame;
		cv::VideoCapture cap;
		void (*process)(cv::Mat_<cv::Vec3b>&, cv::Mat_<cv::Vec3b>&) = nullptr;
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
