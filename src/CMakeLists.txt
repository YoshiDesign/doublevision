cmake_minimum_required(VERSION 2.8)
project( dvision )
find_package( OpenCV REQUIRED )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(gpiolib)
add_subdirectory(visionlib)
add_subdirectory(asio118)

SET_TARGET_PROPERTIES(asio PROPERTIES LINKER_LANGUAGE CXX)

add_executable( ${PROJECT_NAME} main.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} vision-cpp gpio-cpp gpiod asio)
