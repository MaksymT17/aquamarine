cmake_minimum_required (VERSION 2.6)

project (aquamarine_ut)

set(CMAKE_PROJECT_VERSION 0.7)
set(CMAKE_PROJECT_VERSION_MAJOR ${CMAKE_PROJECT_VERSION})
set(CMAKE_PROJECT_VERSION_MINOR ${CMAKE_PROJECT_VERSION})
set(CMAKE_PROJECT_VERSION_PATCH ${CMAKE_PROJECT_VERSION})
set(AM_SRC ../)
set(AM_INC ../)

add_compile_options(-fno-rtti)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O1 -fprofile-arcs -ftest-coverage")

find_package (Threads REQUIRED)
IF (UNIX)
find_package (SQLite3 REQUIRED)
find_package (JPEG REQUIRED)
ENDIF()

# Set the CMAKE_CXX_STANDARD variable to 17
set(CMAKE_CXX_STANDARD 17)

# Set the CMAKE_CXX_STANDARD_REQUIRED variable to ON
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${SQLite3_INCLUDE_DIRS} ${JPEG_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${AM_INC} ${AM_INC}/sh_mem)
IF (UNIX)
include_directories(${SQLite3_INCLUDE_DIRS} ${JPEG_INCLUDE_DIR})
ENDIF()

#fetch gtest library
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB UT_SOURCES "*.cpp")

add_subdirectory(../sh_mem ${CMAKE_BINARY_DIR}/sh_mem)

add_executable(aquamarine_ut
    ${AM_SRC}/extraction/BmpExtractor.cpp
    ${AM_SRC}/extraction/JpgExtractor.cpp
    ${AM_SRC}/analyze/algorithm/movement/MovementDetector.cpp
    ${AM_SRC}/analyze/algorithm/ImagePair.cpp
    ${AM_SRC}/analyze/algorithm/BfsObjectDetector.cpp
    ${AM_SRC}/analyze/AffinityComparer.cpp 
    ${AM_SRC}/analyze/ThresholdDiffChecker.cpp 
    ${AM_SRC}/analyze/algorithm/ObjectBase.cpp
    ${AM_SRC}/analyze/algorithm/Object.cpp
    ${AM_SRC}/analyze/algorithm/ObjectRectangle.cpp
    ${AM_SRC}/analyze/algorithm/ObjectDetector.cpp 
    ${AM_SRC}/analyze/algorithm/DiffObjectDetector.cpp 
    ${AM_SRC}/extraction/MultipleExtractor.cpp
    ${AM_SRC}/database/DataBaseCommunicator.cpp
    ${UT_SOURCES}
)
target_link_libraries(aquamarine_ut gtest_main shared_mem ${CMAKE_THREAD_LIBS_INIT})
IF (UNIX)
target_link_libraries(aquamarine_ut ${SQLite3_LIBRARIES} ${JPEG_LIBRARIES} )
ENDIF()
# gcov - coverage
set(CMAKE_COMPILER_IS_GNUCXX true)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CodeCoverage)
setup_target_for_coverage_gcovr_html(
    NAME coverage_aquamarine_ut
    EXECUTABLE aquamarine_ut
    BASE_DIRECTORY "${AM_SRC}/"
    EXCLUDE "${CMAKE_SOURCE_DIR}/" "${CMAKE_SOURCE_DIR}/../thirdparty/"
)
