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

# pkg_check_modules(GTK REQUIRED "gtk+-3.0")
# if (NOT (GTK_FOUND))
#     message(FATAL_ERROR "Please install GTK 3.0: CMake will Exit")
# endif()

pkg_check_modules(GSTVIDEO REQUIRED gstreamer-video-1.0)
if (NOT (GSTVIDEO_FOUND))
    message(FATAL_ERROR "Please install gst-video: CMake will Exit")
endif()

pkg_check_modules(GSTAUDIO REQUIRED gstreamer-audio-1.0)
if (NOT (GSTAUDIO_FOUND))
    message(FATAL_ERROR "Please install gst-audio: CMake will Exit")
endif()

# pkg_check_modules(GSTDIS REQUIRED gstreamer-pbutils-1.0)
# if (NOT (GSTDIS_FOUND))
#     message(FATAL_ERROR "Please install gst-pbutils: CMake will Exit")
# endif()


message(STATUS "include => ${GST_INCLUDE_DIRS}")
message(STATUS "include => ${GTK_INCLUDE_DIRS}")
message(STATUS "include => ${GSTVIDEO_INCLUDE_DIRS}")
message(STATUS "include => ${GSTAUDIO_INCLUDE_DIRS}")
message(STATUS "include => ${GSTDIS_INCLUDE_DIRS}")

message(STATUS "lib => ${GST_LIBRARY_DIRS}")
message(STATUS "lib => ${GTK_LIBRARY_DIRS}")
message(STATUS "lib => ${GSTVIDEO_LIBRARY_DIRS}")
message(STATUS "lib => ${GSTAUDIO_LIBRARY_DIRS}")
message(STATUS "lib => ${GSTDIS_LIBRARY_DIRS}")

include_directories(
    ${GST_INCLUDE_DIRS} 
    ${GTK_INCLUDE_DIRS}
    ${GSTVIDEO_INCLUDE_DIRS}
    ${GSTAUDIO_INCLUDE_DIRS}
    ${GSTDIS_INCLUDE_DIRS}
    )
link_directories(
    ${GST_LIBRARY_DIRS} 
    ${GTK_LIBRARY_DIRS}
    ${GSTVIDEO_LIBRARY_DIRS}
    ${GSTAUDIO_LIBRARY_DIRS}
    ${GSTDIS_LIBRARY_DIRS}
    )

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

# tutorials
list(APPEND srcs src/time4.c 
            src/basic3.c
            src/basic2.c
            src/basic1.c
            # src/gui5.c
            src/padcap6.c
            src/thread7.c
            src/shortcutting8.c
            src/info9.c
            # src/demosrc.c
            )
list(APPEND prjs time4
            tutorialthree
            tutorialtwo
            tutorialone
            # gui5
            padcap6
            thread7
            shortcutting8
            info9
            # demosrc
            )
foreach(file prj IN ZIP_LISTS srcs prjs)
    message(STATUS ${prj} " - " ${file})
    add_executable(${prj}
        ${PROJECT_SOURCE_DIR}/${file}
    )
    target_include_directories(
        ${prj}
        PRIVATE
        ${GST_INCLUDE_DIRS}
        ${GTK_INCLUDE_DIRS}
        ${GSTVIDEO_INCLUDE_DIRS}
        ${GSTAUDIO_INCLUDE_DIRS}
        ${GSTDIS_INCLUDE_DIRS}
    )
    target_link_libraries(
        ${prj}
        ${GST_LIBRARIES}
        ${GTK_LIBRARIES}
        ${GSTVIDEO_LIBRARIES}
        ${GSTAUDIO_LIBRARIES}
        ${GSTDIS_LIBRARIES}
    )
endforeach()


########################################################

if(CMAKE_SYSTEM STREQUAL "Linux")
    message(STATUS "Configuring on Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Configuring on macOS")
else()
    message(STATUS "Unsupported os")
    
endif()