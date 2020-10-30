cmake_minimum_required(VERSION 2.8)
project( dvision )
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(gpiolib)
add_subdirectory(visionlib)

add_executable( ${PROJECT_NAME} main.cpp )

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} vision-cpp gpio-cpp gpiod)
