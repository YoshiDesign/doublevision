#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

namespace vis {

	namespace camera {
		class Camera{
		
		public:
			Camera();
			Camera(int device);
			void run(const char* type);
			bool readNextFrame(cv::Mat& frame);

			/*
			 * Assigns a pointer to a callback fn
			 * (Mat& in, Mat& out)
			 */ 
			void setFrameProcessor(void(*frameProCallback)(cv::Mat&, cv::Mat&))

		private:
			int device, useProcess;
			cv::Mat frame;
			cv::Mat outputFrame;
			cv::VideoCapture cap;
			FrameProcessor *frameproc;
			void (*process)(cv::Mat&, cv::Mat&);
			// void (Camera::*process)(const char*);
			std::vector<std::string> images;

		};
		
		/* @class FrameProcessor
		 * This class is only used to supply a callback function to Camera::(*process) function pointer
		 */
		class FrameProcessor {
		
		public: 
			virtual void process(cv::Mat& input, cv::Mat& output) = 0;
		
		};
	}

	namespace image {
		class Image{
		public:
			Image();
			Image(cv::Mat& img);

		private:
			cv::Mat img;
		};	
	
	}
}
