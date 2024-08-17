cmake_minimum_required (VERSION 3.0)

project (shared_mem)

ADD_LIBRARY( shared_mem STATIC
             ProcCommunicator.cpp SharedMemoryReceiver.cpp SharedMemorySender.cpp )