# 编译EXE,DLL不依赖运行库
if(MSVC)     
    # Use the static C library for all build types
    foreach(var 
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
      )
      if(${var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}") 
      endif()
    endforeach()
endif(MSVC)

#设置webrtc路径 需要设置系统环境变量
if (NOT $ENV{WEBRTC} STREQUAL "")
    set(WEBRTC_ROOT $ENV{WEBRTC} CACHE INTERNAL "Copied from environment variable")
endif()
message(STATUS "webrtc_root = ${WEBRTC_ROOT}")
include_directories(${WEBRTC_ROOT})
include_directories(${WEBRTC_ROOT}/third_party/abseil-cpp)
include_directories(${WEBRTC_ROOT}/third_party)

# 生成的DLL,EXE文件带PDB调试信息
if (WIN32)
        string(REPLACE "/Ob2" "/Ob1" out "${CMAKE_CXX_FLAGS_RELEASE}")
        string(REPLACE "/Ob2" "/Ob1" out "${CMAKE_C_FLAGS_RELEASE}")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DRELEASE")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /DRELEASE")
		SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS") 
endif()

# 增加宏定义
ADD_DEFINITIONS(-DNOMINMAX
-DWEBRTC_WIN
-DUNICODE
-D_UNICODE)

# add_definitions(-DUNICODE -D_UNICODE)
# ADD_DEFINITIONS(-D_TEST_)

# 包括目录头文件
# include_directories(${protest}/test/include)

# 自动搜索指定目录下的文件列到目录下
# file(GLOB_RECURSE PUBLIC_HDR_DIR *.h ${protest}/test/include/*.h ${protest}/protest/include/*.h)
# file(GLOB_RECURSE SRCS  *.cpp ${protest}/test/src/*.cpp  ${protest}/test/src/*.cpp)

# 设置编译器版本
# set(CXX_STANDARD "-std=c++14")

set(CALLDIR ./call/calltest.h
./call/calltest.cpp
call/audioloop.h
call/audioloop.cpp)

source_group(call ${CALLDIR})
# 查找文件所有的.h和.cpp
FILE(GLOB SC_FILES "*.cpp" "*.h")
set(RESOURCE small.ico
webrtc_test.aps
webrtc_test.ico
webrtc_test.rc)

#aux_source_directory(. DIRSRC)
#设置链接库路径
LINK_DIRECTORIES(${WEBRTC_ROOT}/out/Default/obj)
#设置链接库名称
LINK_LIBRARIES(webrtc Strmiids winmm ws2_32 dmoguids wmcodecdspuuid Msdmo)
#target_link_libraries(Strmiids)
add_executable(webrtc_test ${SC_FILES} ${RESOURCE} ${CALLDIR}
)
