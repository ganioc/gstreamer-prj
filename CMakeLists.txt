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
message(STATUS "${PROJECT_SOURCE_DIR}")
#  only help in finding header files
include_directories("${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
    )

message(STATUS ${GST_INCLUDE_DIRS})
target_include_directories(
    ${CMAKE_PROJECT_NAME} PRIVATE 
    ${GST_INCLUDE_DIRS}
)

message(STATUS ${GST_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME}  
    ${GST_LIBRARIES})

# tutorial 1, basic play a webm,
set(PRJ1 "tutorialone")
add_executable(${PRJ1}  
    ${PROJECT_SOURCE_DIR}/src/basic1.c)

target_include_directories(
    ${PRJ1} PRIVATE 
    ${GST_INCLUDE_DIRS}
)
target_link_libraries(${PRJ1}  
    ${GST_LIBRARIES})

# tutorial 2, 
set(PRJ2 "tutorialtwo")
add_executable(${PRJ2}  
    ${PROJECT_SOURCE_DIR}/src/basic2.c)

target_include_directories(
    ${PRJ2} PRIVATE 
    ${GST_INCLUDE_DIRS}
)
target_link_libraries(${PRJ2}  
    ${GST_LIBRARIES})
########################################################

if(CMAKE_SYSTEM STREQUAL "Linux")
    message(STATUS "Configuring on Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Configuring on macOS")
else()
    message(STATUS "Unsupported os")
    
endif()