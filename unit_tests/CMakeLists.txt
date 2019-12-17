cmake_minimum_required (VERSION 2.6)

project (aquamarine_ut)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package (Threads)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(..)


add_executable(bmpExtraction_ut 
TestBase.hpp
../extraction/BmpExtractor.cpp
BmpReaderTest.cpp
)

add_executable(movementDetector_ut 
../extraction/BmpExtractor.cpp
../analyze/algorithm/movement/MovementDetector.cpp
../analyze/algorithm/Object.cpp
../analyze/algorithm/ImagePair.cpp
../analyze/algorithm/BfsObjectDetector.cpp
../analyze/algorithm/ObjectDetector.cpp
MovementDetectorTest.cpp
)

add_executable(objectDetection_ut
../analyze/AffinityComparer.cpp 
../analyze/ThresholdDiffChecker.cpp 
../analyze/algorithm/Object.cpp
../analyze/algorithm/BfsObjectDetector.cpp 
../analyze/algorithm/ObjectDetector.cpp 
../analyze/algorithm/DiffObjectDetector.cpp 
../analyze/algorithm/ImagePair.cpp 
../analyze/algorithm/movement/MovementDetector.cpp
../extraction/BmpExtractor.cpp
../extraction/MultipleBmpExtractor.cpp
../common/Timers.hpp
ObjectDetectorTest.cpp
)
add_executable(matrix_ut 
MatrixTest.cpp
)

target_link_libraries (movementDetector_ut ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (objectDetection_ut ${CMAKE_THREAD_LIBS_INIT})

