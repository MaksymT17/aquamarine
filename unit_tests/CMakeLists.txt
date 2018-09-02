cmake_minimum_required (VERSION 2.6)

project (aquamarine_ut)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package (Threads)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_executable(aquamarine_ut 
TestBase.hpp
)

target_link_libraries (aquamarine ${CMAKE_THREAD_LIBS_INIT})

