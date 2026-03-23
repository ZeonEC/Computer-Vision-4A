#------------------------------------------------#
#  CMake configuration for Computer Vision project
#------------------------------------------------#

# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.16)

# Nom du projet et langage utilisé
project(Computer-Vision-4A LANGUAGES CXX)

# Standard C++ utilisé
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tous les executables seront générés dans le dossier build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# -------------------------------------------------
# Trouver OpenCV installé sur le système
# -------------------------------------------------
find_package(OpenCV REQUIRED)

# -------------------------------------------------
# Dossiers d'include du projet
# -------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# -------------------------------------------------
# Fichiers sources
# -------------------------------------------------
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
)

# -------------------------------------------------
# Création de l'exécutable main
# -------------------------------------------------
add_executable(main ${SOURCES} )

# -------------------------------------------------
# Link avec OpenCV
# -------------------------------------------------
target_link_libraries(main ${OpenCV_LIBS})