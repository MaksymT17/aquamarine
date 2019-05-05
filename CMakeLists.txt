cmake_minimum_required (VERSION 2.6)

project (aquamarine)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package (Threads)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_executable(aquamarine 
main.cpp 
analyze/AffinityComparer.cpp 
analyze/ThresholdDiffChecker.cpp 
analyze/algorithm/Object.cpp
analyze/algorithm/BfsObjectDetector.cpp 
analyze/algorithm/ObjectDetector.cpp 
analyze/algorithm/DiffObjectDetector.cpp 
analyze/algorithm/ImagePair.cpp 
analyze/algorithm/movement/MovementDetector.cpp
extraction/BmpExtractor.cpp
extraction/MultipleBmpExtractor.cpp
common/Timers.hpp
)

target_link_libraries (aquamarine ${CMAKE_THREAD_LIBS_INIT})

