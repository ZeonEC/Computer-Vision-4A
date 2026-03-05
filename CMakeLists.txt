#------------------------------------------------#
# Fichier CMakeLists.txt pour le projet Computer Vision
#------------------------------------------------#

cmake_minimum_required(VERSION 3.16)

project(Computer-Vision-4A LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tous les exécutables iront dans build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# (Optionnel) glob de sources si tu veux ajouter plusieurs .cpp automatiquement
file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

#--- OpenCV ---#
find_package(OpenCV REQUIRED)

# Crée l'exécutable
add_executable(main
  ${SRC}          # ou juste src/main.cpp si tu préfères explicite
)

# Includes de ton projet + OpenCV
target_include_directories(main PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${OpenCV_INCLUDE_DIRS}
)

# Link OpenCV
target_link_libraries(main PRIVATE
  ${OpenCV_LIBS}
)