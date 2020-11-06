#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <queue>
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

	void operator()( int line_num, gpiod::chip& chip, std::queue<int>& Q ){

		// Hold the line
		gpiod::line line0( chip.get_line(line_num) );
		gpiod::line_request lReq;

		// Define the line request
		lReq.request_type = lReq.EVENT_RISING_EDGE;

		// Release the line in case it held anything
		//line0.release();

		// Stream for USB -> Arduino
		std::ofstream outStream;
		outStream.open("/dev/ttyACM0");
		// Connect to the Arduino
	
		// Wait for analog input	
		for(;;) 
		{
			// Define a line request
			line0.request(lReq);

			if ( line0.event_wait(std::chrono::nanoseconds(sec)) )
			{

				// Trigger the injection of the current matrix
				Q.push(1);	
				
				// Send Signal to Camera and drop the line
				line0.release();
			}

		}
	}

};

struct CameraEvent {

	void operator()(vis::camera::Camera& cam, std::queue<int>& Q){
	
		cam.run(Q, "default");
	
	}

};

struct DataChannel {

	cv::Mat* data;
	
	void send()
	{
		// Serialize the data and send it to the Arduino
		std::cout << "IN: ";
		//std::cin >> degrees;
		//std::string ss = std::to_string(degrees);
		//outStream << ss << std::endl;

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

	// Camera
	vis::camera::Camera cam = vis::camera::Camera();

	// Chip shoutout
	std::cout << "Chip:\t" << chip0.name() << std::endl;

	// Signal queue - Queue for Extensible reasons
	std::queue<int> Q;

	// GPIO Event Listner w/ Q
	SingleEventInput evt;

	// The camera's run() thread w/ Q
	CameraEvent camEvt;

	std::thread t { [&chip0, &evt, &Q] { evt(17, chip0, Q); } };
	std::thread c { [&camEvt, &Q, &cam] { camEvt(cam, Q); } };
	t.join();
	c.join();

	return 0;
	
}
