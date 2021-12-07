cmake_minimum_required (VERSION 2.6)

project (aquamarine_ut)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package (Threads)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(..)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
#add_executable(example example.cpp)
#target_link_libraries(example gtest_main)
#add_test(NAME example_test COMMAND example)

add_executable(bmpExtraction_ut 
    TestBase.hpp
    ../extraction/BmpExtractor.cpp
    BmpReaderTest.cpp
)
target_link_libraries(bmpExtraction_ut gtest_main)
add_test(NAME example_test COMMAND bmpExtraction_ut)



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

