cmake_minimum_required(VERSION 2.8.9)
project (BASELIB)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")
add_definitions( -D__STDC_CONSTANT_MACROS -D__UCLIBC__   -D__STDC_FORMAT_MACROS)
if(ANDROID)
add_definitions(-DWEBRTC_ANDROID)
elseif(APPLE)
#set(SUPPORTED_PLATFORMS "iphoneos iphonesimulator")
message("build type is ${CMAKE_BUILD_TYPE}")
add_definitions(-DWEBRTC_IOS -DWEBRTC_POSIX)
#SET (IPHONE_SDKROOT "${IPHONE_DEVROOT}/SDKs/iPhoneSimulator${IPHONE_SDKVER}.sdk")
endif()
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")	
#base
set (BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/webrtc/base)
set (BASE_SOURCE_FILES  ${BASE_DIR}/sigslot.cc
						${BASE_DIR}/sigslot.h
                        ${BASE_DIR}/basictypes.h
						${BASE_DIR}/thread.h
						${BASE_DIR}/thread.cc
						${BASE_DIR}/constructormagic.h
						${BASE_DIR}/event.h
						${BASE_DIR}/event.cc
						${BASE_DIR}/messagequeue.h
						${BASE_DIR}/messagequeue.cc
						${BASE_DIR}/checks.cc
						${BASE_DIR}/checks.h
						${BASE_DIR}/socketserver.h
						${BASE_DIR}/nullsocketserver.h
						${BASE_DIR}/nullsocketserver.cc
						${BASE_DIR}/logging.cc
						${BASE_DIR}/logging.h
						${BASE_DIR}/location.h
						${BASE_DIR}/location.cc
						${BASE_DIR}/criticalsection.h
						${BASE_DIR}/criticalsection.cc
						${BASE_DIR}/timeutils.h
						${BASE_DIR}/timeutils.cc
						${BASE_DIR}/platform_thread.h
						${BASE_DIR}/platform_thread.cc
						${BASE_DIR}/event_tracer.h
						${BASE_DIR}/event_tracer.cc
						${BASE_DIR}/format_macros.h
						${BASE_DIR}/sharedexclusivelock.h
						${BASE_DIR}/sharedexclusivelock.cc
						${BASE_DIR}/messagehandler.h
						${BASE_DIR}/messagehandler.cc
						${BASE_DIR}/stringencode.h
						${BASE_DIR}/stringencode.cc
						${BASE_DIR}/asyncsocket.h
						${BASE_DIR}/asyncsocket.cc
						${BASE_DIR}/socketaddress.h
						${BASE_DIR}/socketaddress.cc
						${BASE_DIR}/ifaddrs-android.h
						${BASE_DIR}/ifaddrs-android.cc
						${BASE_DIR}/ipaddress.h
						${BASE_DIR}/ipaddress.cc
						${BASE_DIR}/nethelpers.h
						${BASE_DIR}/nethelpers.cc
						${BASE_DIR}/asyncfile.h
						${BASE_DIR}/asyncfile.cc
						${BASE_DIR}/signalthread.cc
						${BASE_DIR}/asyncresolverinterface.cc
						${BASE_DIR}/bind.h
						${BASE_DIR}/asyncinvoker.h
						${BASE_DIR}/asyncinvoker.cc
						${BASE_DIR}/thread_checker.h
						${BASE_DIR}/thread_checker_impl.cc
						${BASE_DIR}/thread_checker_impl.h
						${BASE_DIR}/common.cc
						${BASE_DIR}/common.h
						${BASE_DIR}/physicalsocketserver.h
						${BASE_DIR}/physicalsocketserver.cc
					)
if(ANDROID)
set (BASE_SOURCE_FILES  ${BASE_SOURCE_FILES} 
						${BASE_DIR}/physicalsocketserver.h
						${BASE_DIR}/physicalsocketserver.cc)
set(BASEANDROID_DIR ${CMAKE_CURRENT_SOURCE_DIR}/webrtc/android)					
file(GLOB  BASE_ANDROIDSOURCE_FILES ${BASEANDROID_DIR}/jnihelper/*.cpp
							   ${BASEANDROID_DIR}/jnihelper/*.h)
endif()
add_library( baselib
             STATIC
             ${BASE_SOURCE_FILES} ${BASE_ANDROIDSOURCE_FILES} )
			 
if(ANDROID)
# 找到预编译库 log_lib 并link到我们的动态库 native-lib中
find_library( # Sets the name of the path variable.
              log-lib
              log )			 
target_link_libraries( baselib
					  ${log-lib} )
endif()
if(ANDROID)
# set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/android)
else(IOS)
# set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/ios)
endif()				  
INSTALL(TARGETS baselib
				LIBRARY DESTINATION lib
				ARCHIVE DESTINATION lib)	
#ADD_CUSTOM_COMMAND(TARGET baselib

#COMMAND   ${CMAKE_STRIP} *.so )				