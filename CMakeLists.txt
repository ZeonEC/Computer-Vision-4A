#------------------------------------------------#
# Fichier CMakeLists.txt pour le projet Computer Vision
#------------------------------------------------#

cmake_minimum_required(VERSION 3.16)

project(Computer-Vision-4A LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tous les exécutables iront dans build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Crée l'exécutable
add_executable(main
  ${SRC}          # ou juste src/main.cpp si tu préfères explicite
)