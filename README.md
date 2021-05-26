# doublevision

A C++ library with the following tools at your disposal:

- Networking with ASIO (In Progress)
- GPIO with libgpiod
- Computer Vision with OpenCV2

CMakeLists.txt included for easy compiling.

### Dynamic Libraries

The OpenCV modules are assumed to be dynamically linked. Simply installing OpenCV on your machine will add these files to your linker's search path.

### Static Libraries

Libgpiod and ASIO are included as static libraries.
