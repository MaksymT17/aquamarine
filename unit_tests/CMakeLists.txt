cmake_minimum_required (VERSION 2.6)

project (aquamarine_ut)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

#find_package (Threads)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(..)


add_executable(bmpExtraction_ut 
TestBase.hpp
../extraction/BmpExtractor.cpp
BmpReaderTest.cpp
main.cpp
)

#add_executable(movementDetector_ut 
#../analyze/algorithm/movement/MovementDetector.cpp
#../analyze/algorithm/Object.cpp
#../analyze/algorithm/ImagePair.cpp
#../analyze/algorithm/BfsObjectDetector.cpp
#../analyze/algorithm/ObjectDetector.cpp
#MovementDetectorTest.cpp
#)

target_link_libraries (aquamarine_ut ${CMAKE_THREAD_LIBS_INIT})

