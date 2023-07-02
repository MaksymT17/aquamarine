cmake_minimum_required (VERSION 2.6)

project (aquamarine)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
add_compile_options(-fno-rtti)

find_package (Threads)
find_package (SQLite3)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${SQLite3_INCLUDE_DIRS})

set(AM_SOURCE_FILES 
    analyze/AffinityComparer.cpp 
    analyze/ThresholdDiffChecker.cpp 
    analyze/algorithm/ObjectBase.cpp
    analyze/algorithm/Object.cpp
    analyze/algorithm/ObjectRectangle.cpp
    analyze/algorithm/BfsObjectDetector.cpp 
    analyze/algorithm/ObjectDetector.cpp 
    analyze/algorithm/DiffObjectDetector.cpp 
    analyze/algorithm/ImagePair.cpp 
    analyze/algorithm/movement/MovementDetector.cpp
    extraction/BmpExtractor.cpp
    extraction/MultipleBmpExtractor.cpp
    common/Timers.hpp
    database/DataBaseCommunicator.cpp
    AmApi.cpp
)

add_library(aquamarine_lib STATIC ${AM_SOURCE_FILES} )

add_executable(aquamarine main.cpp)

target_link_libraries (aquamarine aquamarine_lib ${CMAKE_THREAD_LIBS_INIT} ${SQLite3_LIBRARIES})

