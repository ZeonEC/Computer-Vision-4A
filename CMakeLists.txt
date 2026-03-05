#------------------------------------------------#
# Fichier CMakeLists.txt pour le projet Computer Vision #
#------------------------------------------------#

#--- Configuration de CMake ---#
cmake_minimum_required(VERSION 3.16) 

project(Computer-Vision-4A LANGUAGES CXX) 
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

# Tous les exécutables iront dans build/ (ou build/bin si tu préfères) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) 
file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp") 

#--- Configuration des libraires annexes ---#


#-------------------------------------------#


# Crée l'exécutable à partir des fichiers sources
add_executable(main
  src/main.cpp
)

#--- Lier les librairies au bon target ---# 

#-------------------------------------------#

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)