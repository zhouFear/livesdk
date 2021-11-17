cmake_minimum_required(VERSION 3.0)

project(livesdk)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=C++11")
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)

# …Ë÷√boost
set(BOOST_INCLUDEDIR " ..\\3rdparty\\boost_1_77_0")
set(BOOST_LIBRARYDIR " ..\\3rdparty\\boost_1_77_0\\stage\\lib")
#set(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF)

set(NETWORK_SRCS 
network/tcp/tcpserver.h
network/tcp/tcpserver.cpp
network/tcp/tcpconnection.h
network/tcp/tcpconnection.cpp
network/tcp/tcpconnectionmgr.h
network/tcp/tcpconnectionmgr.cpp
network/tcp/tcpclient.h
network/tcp/tcpclient.cpp
network/udp/udpserver.h
network/udp/udpserver.cpp
network/udp/udpclient.h
network/udp/udpclient.cpp)

set(LOG_SRCS 
log/logger.h
log/logger.cpp
)

source_group(network FILES ${NETWORK_SRCS})
source_group(log FILES ${LOG_SRCS})

#sub
add_subdirectory(test/tcpclient)
add_subdirectory(test/tcpserver)
add_subdirectory(webrtc_test)

add_definitions(-D DLLEXPORTS_SDK)

find_package(Boost REQUIRED COMPONENTS)
if(Boost_FOUND)
    MESSAGE(STATUS "Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIRS}")
    MESSAGE(STATUS "Boost_LIBRARIES ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    #add_executable(livesdk main.cpp ${NETWORK_SRCS} ${LOG_SRCS})
    add_library(livesdk SHARED dllmain.cpp ${NETWORK_SRCS} ${LOG_SRCS})
    #add_executable(${PROJECT_NAME} ${DIR_SRCS} ${HEADERS})  
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()
