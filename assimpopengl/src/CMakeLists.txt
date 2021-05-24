############################################################
#   The CMake File for assimpopengl library.  It provides for
#   the following libraries:  Boost, GLM, GL, GLEW,
#   stdc++, freeimageplus, SDL2, assimp and pthread.
#   Created by Edward C. Eberle <eberdeed@ebedeed.net>
#   April 2020 San Diego California USA
############################################################
cmake_minimum_required(VERSION 3.12)
project(assimpopengl VERSION 1.0)
add_library(assimpopengl SHARED camera.cpp model.cpp mesh.cpp shader.cpp createimage.cpp smokecloud.cpp uniformprinter.cpp)
set_target_properties(assimpopengl PROPERTIES SOVERSION 1.0)
add_definitions(-g -fPIC -std=c++17 -pthread)
include_directories(/usr/include/GL /usr/include/boost /usr/include/glm /usr/include/assimp /usr/include/SDL2 /usr/local/include)
link_directories(/usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib)
target_link_libraries(assimpopengl stdc++ GLEW GL GLU pthread boost_chrono boost_filesystem boost_system assimp freeimage 
freeimageplus X11 pthread SDL2-2.0 pthread)
install(TARGETS assimpopengl DESTINATION /usr/lib)
