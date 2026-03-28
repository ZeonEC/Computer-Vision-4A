#------------------------------------------------#
#  CMake configuration for Computer Vision project
#------------------------------------------------#

cmake_minimum_required(VERSION 3.16)

project(Computer-Vision-4A LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(OpenCV REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

set(SOURCES
    src/main.cpp
    src/menu.cpp
    src/capture_cam.cpp
    src/calibration.cpp
    src/verification_calibration.cpp
    src/gestion_XML.cpp
    src/stereo_calibration.cpp
    src/stereo_capture_cam.cpp 
    src/verif_stereo_calib.cpp
    sr/testchessbord.cpp
)

add_executable(main ${SOURCES})

target_link_libraries(main ${OpenCV_LIBS})

# 🔹 Désactive les warnings des librairies externes
target_compile_options(main PRIVATE /W0)