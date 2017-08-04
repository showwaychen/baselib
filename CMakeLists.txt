cmake_minimum_required(VERSION 2.8.9)
project (BASELIB)
#base
set (BASE_SOURCE_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/arraysize.h 
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h
						${CMAKE_CURRENT_SOURCE_DIR}/base/array_view.h)

add_library( baselib
             STATIC
             ${PLAYER_SOURCES_FILES} )