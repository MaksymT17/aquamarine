cmake_minimum_required (VERSION 2.6)

project (aquamarine_ut)
set(CMAKE_PROJECT_VERSION 0.7)
set(CMAKE_PROJECT_VERSION_MAJOR ${CMAKE_PROJECT_VERSION})
set(CMAKE_PROJECT_VERSION_MINOR ${CMAKE_PROJECT_VERSION})
set(CMAKE_PROJECT_VERSION_PATCH ${CMAKE_PROJECT_VERSION})
set(AM_SRC ..)
set(AM_INC ..)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package (Threads)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${AM_INC})

#fetch gtest library
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(bmpExtraction_ut 
    TestBase.hpp
    ${AM_SRC}/extraction/BmpExtractor.cpp
    BmpReaderTest.cpp
)
target_link_libraries(bmpExtraction_ut gtest_main)

add_executable(movementDetector_ut 
  ${AM_SRC}/extraction/BmpExtractor.cpp
  ${AM_SRC}/analyze/algorithm/movement/MovementDetector.cpp
  ${AM_SRC}/analyze/algorithm/Object.cpp
  ${AM_SRC}/analyze/algorithm/ImagePair.cpp
  ${AM_SRC}/analyze/algorithm/BfsObjectDetector.cpp
  ${AM_SRC}/analyze/algorithm/ObjectDetector.cpp
  MovementDetectorTest.cpp
)
target_link_libraries(movementDetector_ut gtest_main)

add_executable(objectDetection_ut
  ${AM_SRC}/analyze/AffinityComparer.cpp 
  ${AM_SRC}/analyze/ThresholdDiffChecker.cpp 
  ${AM_SRC}/analyze/algorithm/Object.cpp
  ${AM_SRC}/analyze/algorithm/BfsObjectDetector.cpp 
  ${AM_SRC}/analyze/algorithm/ObjectDetector.cpp 
  ${AM_SRC}/analyze/algorithm/DiffObjectDetector.cpp 
  ${AM_SRC}/analyze/algorithm/ImagePair.cpp 
  ${AM_SRC}/analyze/algorithm/movement/MovementDetector.cpp
  ${AM_SRC}/extraction/BmpExtractor.cpp
  ${AM_SRC}/extraction/MultipleBmpExtractor.cpp
  ${AM_SRC}/common/Timers.hpp
  ObjectDetectorTest.cpp
)
target_link_libraries(objectDetection_ut gtest_main)

add_executable(matrix_ut 
  MatrixTest.cpp
)
target_link_libraries(matrix_ut gtest_main)

target_link_libraries (movementDetector_ut ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (objectDetection_ut ${CMAKE_THREAD_LIBS_INIT})

