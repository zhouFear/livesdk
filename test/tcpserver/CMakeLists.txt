cmake_minimum_required(VERSION 3.0)

set(BOOST_INCLUDEDIR " ..\\..\\..\\3rdparty\\boost_1_77_0")
include_directories(${Boost_INCLUDE_DIRS})
set(SRC main.cpp)

ADD_EXECUTABLE(tcpserver ${SRC})
