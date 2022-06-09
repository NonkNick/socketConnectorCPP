cmake_minimum_required(VERSION 3.13)
project(socketConnector)

set(CMAKE_CXX_STANDARD 14)

add_executable(start.cpp start.h randomValues.cpp randomValues.h socketConnector.cpp socketConnector.h)