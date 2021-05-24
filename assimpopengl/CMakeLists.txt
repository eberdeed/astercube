############################################################
#   The CMake File for assimpopengl library.  It provides for
#   the following libraries:  Boost, GLM, GL, GLEW,
#   stdc++, freeimageplus, SDL2, assimp and pthread.
#   Created by Edward C. Eberle <eberdeed@ebedeed.net>
#   April 2020 San Diego California USA
############################################################
cmake_minimum_required(VERSION 3.12)
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()
project(assimpopengl)
add_subdirectory(src)
add_subdirectory(include)
# Require dot, treat the other components as optional
find_package(Doxygen REQUIRED dot)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN YES)
set(PROJECT_NAME "Assimp OpenGL")
set(PROJECT_BRIEF "A library to display a arbitrary Blender objects.")
doxygen_add_docs(assimpdocs include src COMMENT "Generating documentation.")
install(DIRECTORY LGPL DESTINATION /usr/share/doc/assimpopengl-doc-1.0 FILE_PERMISSIONS WORLD_READ)
install(DIRECTORY build/html DESTINATION /usr/share/doc/assimpopengl-doc-1.0 FILE_PERMISSIONS WORLD_READ)
install(DIRECTORY build/man DESTINATION /usr/share/doc/assimpopengl-doc-1.0 FILE_PERMISSIONS WORLD_READ)
install(FILES README.txt CHANGELOG.txt DESTINATION /usr/share/doc/assimpopengl-doc-1.0 PERMISSIONS WORLD_READ)
