cmake_minimum_required(VERSION 4.0)
project(RangeAllocator)

set(CMAKE_CXX_STANDARD 20)

add_executable(RangeAllocator main.cpp
        BinaryTracker.cpp
        BinaryTracker.h
        BinaryTrackerNode.cpp
        BinaryTrackerNode.h
        NodeRequestHandler.cpp
        NodeRequestHandler.h
)