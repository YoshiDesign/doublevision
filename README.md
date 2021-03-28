# doublevision

A C++ library with the following tools at your disposal:

- Networking with ASIO (In Progress)
- GPIO with libgpiod
- Computer Vision with OpenCV2

CMakeLists.txt included for easy compiling.

### Dynamic Libraries

The OpenCV modules are assumed to be dynamically linked. You'll have to add any OpenCV2 modules to your compiler's linked libraries.

### Static Libraries

Libgpiod and ASIO are included as static libraries.
