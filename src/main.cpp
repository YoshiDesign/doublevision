#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include "gpiolib/gpiod.hpp"
#include "visionlib/visionlib.hpp"

#define GPIO_0	"gpiochip0"
#define GPIO_1	"gpiochip1"

#define DEVICE_ID	0

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

int main(int argc, char** argv )
{
	// Time Reference Helpers
	// auto timeNow = std::chrono::system_clock::now();
	// Nanoseconds
	// auto duration = timeNow.time_since_epoch();	
	
	// For 10 second ratios 
	std::chrono::seconds sec(1000);

	/**
	 * Open the chip and get a line
	 * - chip.open has built-in exception handling 
	 * - 2nd arg defaults to chip.OPEN_LOOKUP
	 * 	 which guesses where to find it. But I know
	 *   the name of the chip
	 */ 
	gpiod::chip chip0("gpiochip0");

	// Blink -- Output
	gpiod::line line0(chip0.get_line(18));
	// Input -- Button
	gpiod::line line1(chip0.get_line(17));

// 	ALT
//	line0 = chip0.get_line(17);

	std::cout << "Chip:\t" << chip0.name() << std::endl;
	std::cout << "Inspect Line" << std::endl;

	inspect_line(line0);
	inspect_line(line1);

	/**
	 * Construct a line request struct for line0
	 * 
	 */ 
	gpiod::line_request lReq0;
	gpiod::line_request lReq1;

	lReq0.request_type =lReq0.DIRECTION_OUTPUT;
	lReq1.request_type =lReq1.EVENT_RISING_EDGE;

	/**
	 * This sets a line with the behavior defined
	 * by our line request structs
	 */ 
	line0.request(lReq0);
	line1.request(lReq1);

	std::cout << "Waiting... " << std::endl;

	// Wait for button press
	if(line1.event_wait(std::chrono::nanoseconds(sec)))
	{
		line0.set_value(1);	
	}
	
	vis::camera::Camera cam = vis::camera::Camera();
	cam.run("edge");	

	return 0;
	
}
