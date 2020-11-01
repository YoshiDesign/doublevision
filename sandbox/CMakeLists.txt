cmake_minimum_required(VERSION 2.8)
project( sandbox )
include_directories( /usr/local/include/libusb-1.0 )
add_executable( ${PROJECT_NAME} main.cpp )
