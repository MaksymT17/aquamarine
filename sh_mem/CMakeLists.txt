cmake_minimum_required (VERSION 3.0)

project (shared_mem)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)  # Turn off compiler-specific extensions

ADD_LIBRARY( shared_mem STATIC
             ProcCommunicator.cpp SharedMemoryReceiver.cpp SharedMemorySender.cpp )

add_executable(sh_mem_test ProcCommunicator.cpp SharedMemoryReceiver.cpp SharedMemorySender.cpp main.cpp)