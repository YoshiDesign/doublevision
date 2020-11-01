#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
// #include <fcntl.h>
#include <fstream>
// #include <libusb-1.0/libusb.h>

int main()
{

	std::ofstream outdata;
	outdata.open("/dev/ttyACM0");
	if (!outdata)
	{
		std::cout << "Error" << std::endl;
	}


		
	while(1){
		int s;	
		std::cout<< "Degs: ";
		std::cin >> s;

		std::string ss = std::to_string(s);

		outdata << ss << std::endl;

//		char byte;
//		int fd = open("/dev/ttyACM0", O_RDWR);
//		write(fd, s, 4);
		// ssize_t size = read(fd, &byte, 120);

	}
	return 0;
}
