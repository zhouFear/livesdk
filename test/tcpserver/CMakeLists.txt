cmake_minimum_required(VERSION 3.0)

# …Ë÷√boost
set(BOOST_INCLUDEDIR " ..\\3rdparty\\boost_1_77_0")
set(BOOST_LIBRARYDIR " ..\\3rdparty\\boost_1_77_0\\stage\\lib")
#set(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF)

#set(BOOST_INCLUDEDIR " ..\\..\\..\\3rdparty\\boost_1_77_0")
#include_directories(${Boost_INCLUDE_DIRS})
set(SRC main.cpp)

find_package(Boost REQUIRED COMPONENTS)
if(Boost_FOUND)
    MESSAGE(STATUS "Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIRS}")
    MESSAGE(STATUS "Boost_LIBRARIES ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(tcpserver ${SRC})
    #add_executable(${PROJECT_NAME} ${DIR_SRCS} ${HEADERS})  
    target_link_libraries(tcpserver ${Boost_LIBRARIES})
endif()
