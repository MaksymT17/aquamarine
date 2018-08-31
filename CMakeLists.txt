cmake_minimum_required (VERSION 2.6)

project (aq)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package (Threads)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_executable(aq 
main.cpp 
analyze/AffinityComparer.cpp 
analyze/ThresholdDiffChecker.cpp 
analyze/algorithm/Object.cpp
analyze/algorithm/ObjectDetector.cpp 
analyze/algorithm/ImagePair.cpp 
extraction/BmpExtractor.cpp
extraction/MultipleBmpExtractor.cpp
)

target_link_libraries (aq ${CMAKE_THREAD_LIBS_INIT})

