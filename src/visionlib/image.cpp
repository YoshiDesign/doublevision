#include <iostream>
#include <opencv2/opencv.hpp>
#include "visionlib.hpp"

#define LOG(m) std::cout << m << std:endl;

namespace vis { namespace image {
	
	
	Image::Image(){};
	Image::Image(cv::Mat& img): image(img)
	{
	
	};

} // -- namespace image
} // -- namespace vis
