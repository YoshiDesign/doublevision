#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include "gpiolib/gpiod.hpp"
#include "visionlib/visionlib.hpp"

#define GPIO_0	"gpiochip0"
#define GPIO_1	"gpiochip1"

#define DEVICE_ID	0

#define LOG(a) std::cout << a << std::endl

std::chrono::seconds sec(1000);
void sayhi()
{
	std::cout << "Hi!" << std::endl;
}
void inspect_line(gpiod::line& ln)
{
		
	std::cout << "...Inspecting " << ln.name()
		<< "Line Active State:\t" << ln.active_state() 
		<< "\nLine Offset:\t" << ln.offset()
		<< "\nLine Direction:\t" << ln.direction()
		<< "\nLine Consumer:\t" << ln.consumer()
		<< "\nLine is_used:\t" << ln.is_used()
		<< "\nLine is_open_drain:\t" << ln.is_open_drain()
		<< "\nLine is_open_source:\t" << ln.is_open_source()
		<< "\n____________________" << std::endl;


}

struct SingleEventInput {

	/**
	 * Construct a line request
	 */ 
	void operator()( int line_num, gpiod::chip& chip ){
		
		// TODO check if line is in use
		gpiod::line line0( chip.get_line(line_num) );

		// Define a line request	
		gpiod::line_request lReq;
		lReq.request_type =lReq.EVENT_RISING_EDGE;
		line0.request(lReq);

		inspect_line(line0);

		for(;;) 
		{
 			
			if ( line0.event_wait(std::chrono::nanoseconds(sec)) )
			{
				sayhi();
			}
		}
	}

};


int main(int argc, char** argv )
{
	// Time Reference Helpers
	// auto timeNow = std::chrono::system_clock::now();
	// Nanoseconds
	// auto duration = timeNow.time_since_epoch();	
	
	/**
	 * Open the chip and get a line
	 * - chip.open has built-in exception handling 
	 * - 2nd arg defaults to chip.OPEN_LOOKUP
	 * 	 which guesses where to find it. But I know
	 *       the name of the chip
	 */ 
	gpiod::chip chip0("gpiochip0");

	std::cout << "Chip:\t" << chip0.name() << std::endl;

//	lReq0.request_type =lReq0.DIRECTION_OUTPUT;

	/**
	 * This sets a line with the behavior defined
	 * by our line request structs
	 */ 
	std::cout << "Waiting... " << std::endl;

	SingleEventInput evt;
	std::thread t{ [&chip0, &evt] { evt(17, chip0); } };
	vis::camera::Camera cam = vis::camera::Camera();
	cam.run("edge");

	return 0;
	
}
