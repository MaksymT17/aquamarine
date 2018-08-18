cmake_minimum_required (VERSION 2.6)

project (Recognition)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
find_package (Threads)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_executable(recognition 
main.cpp 
analyze/AffinityComparer.cpp 
analyze/ThresholdDiffChecker.cpp 
extraction/BmpExtractor.cpp
)

target_link_libraries (recognition ${CMAKE_THREAD_LIBS_INIT})

