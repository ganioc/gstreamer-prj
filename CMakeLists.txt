cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(RTPGS LANGUAGES C)

# include_directories()
# ${CMAKE_PROJECT_NAME}

message(STATUS ${CMAKE_PROJECT_N})

find_package(PkgConfig REQUIRED)

pkg_check_modules(GST REQUIRED gstreamer-1.0)

message(STATUS "include => ${GST_INCLUDE_DIRS}")
message(STATUS "lib => ${GST_LIBRARIES}")

add_executable(rtpgs
    basic.c
)

target_include_directories(rtpgs PRIVATE 
    ${GST_INCLUDE_DIRS}
)

target_link_libraries(rtpgs PRIVATE 
    ${GST_LIBRARIES})

if(CMAKE_SYSTEM STREQUAL "Linux")
    message(STATUS "Configuring on Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Configuring on macOS")
else()
    message(STATUS "Unsupported os")
    
endif()