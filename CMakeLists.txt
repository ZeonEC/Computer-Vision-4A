cmake_minimum_required(VERSION 3.16)
project(Computer-Vision-4A LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

find_package(OpenCV REQUIRED)

# --- EXE 1 ---
add_executable(main ${SRC})

target_include_directories(main PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(main PRIVATE
  ${OpenCV_LIBS}
)

# --- EXE 2 ---
add_executable(capture_cam
  src/capture_cam.cpp
)
target_include_directories(capture_cam PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(capture_cam PRIVATE
  ${OpenCV_LIBS}
)