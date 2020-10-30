cmake_minimum_required(VERSION 2.8)
project( dvision )
find_package( OpenCV REQUIRED )

add_subdirectory(gpiolib)
add_subdirectory(visionlib)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( dvision main.cpp )

target_link_libraries( dvision ${OpenCV_LIBS} vision-cpp gpio-cpp gpiod)
