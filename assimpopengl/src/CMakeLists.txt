############################################################
#   The CMake File for assimpopengl library.  It provides for
#   the following libraries:  Boost, GLM, GL, GLEW,
#   stdc++, freeimageplus, SDL2, assimp and pthread.
#   Created by Edward C. Eberle <eberdeed@ebedeed.net>
#   April 2020 San Diego California USA
############################################################
cmake_minimum_required(VERSION 3.12)
project(assimpopengl)
add_library(assimpopengl SHARED camera.cpp model.cpp mesh.cpp meshtex.cpp meshvert.cpp shader.cpp createimage.cpp
uniformprinter.cpp)
add_definitions(-g -fPIC -std=c++17 -pthread)
include_directories(/usr/include/GL /usr/include/boost /usr/include/glm /usr/local/include/assimp)
link_directories(/usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib)
target_link_libraries(assimpopengl stdc++ GLEW GL GLU pthread boost_chrono boost_filesystem boost_system assimp freeimage freeimageplus X11 pthread)
install(TARGETS assimpopengl DESTINATION /usr/lib)
