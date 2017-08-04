cmake_minimum_required(VERSION 2.8.9)
project (BASELIB)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")	
#base
set (BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/webrtc/base)
set (BASE_SOURCE_FILES  ${BASE_DIR}/sigslot.cc
					)

add_library( baselib
             SHARED
             ${BASE_SOURCE_FILES} )
			 
# 找到预编译库 log_lib 并link到我们的动态库 native-lib中
find_library( # Sets the name of the path variable.
              log-lib
              log )			 
target_link_libraries( baselib
                      ${log-lib} )
set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/android)					  
INSTALL(TARGETS baselib
				LIBRARY DESTINATION lib)				  