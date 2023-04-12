cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(rtpgs LANGUAGES C)

# include_directories()
# ${CMAKE_PROJECT_NAME}

message(STATUS ${CMAKE_PROJECT_NAME})
message(STATUS ${PROJECT_BINARY_DIR})
message(STATUS ${PROJECT_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
if ( NOT (PKGCONFIG_FOUND))
      message(FATAL_ERROR "Please Install PPkgConfig: CMake will Exit")
endif()

# Streamer
pkg_check_modules(GST REQUIRED gstreamer-1.0)
if ( NOT (GST_FOUND))
      message(FATAL_ERROR "Please Install GStreamer: CMake will Exit")
endif()
set(ENV{PKG_CONFIG_PATH})


message(STATUS "include => ${GST_INCLUDE_DIRS}")
message(STATUS "lib => ${GST_LIBRARY_DIRS}")

include_directories(${GST_INCLUDE_DIRS})
link_directories(${GST_LIBRARY_DIRS})

# find_package(GStreamer REQUIRED)



add_executable(${CMAKE_PROJECT_NAME}
    basic.c
)

include_directories("${PROJECT_SOURCE_DIR}")

message(STATUS ${GST_INCLUDE_DIRS})
target_include_directories(
    ${CMAKE_PROJECT_NAME} PRIVATE 
    ${GST_INCLUDE_DIRS}
)

message(STATUS ${GST_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME}  
    ${GST_LIBRARIES})

########################################################

if(CMAKE_SYSTEM STREQUAL "Linux")
    message(STATUS "Configuring on Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Configuring on macOS")
else()
    message(STATUS "Unsupported os")
    
endif()