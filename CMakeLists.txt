############################################################
#   The CMake File for AsterCube.  It provides for
#   the following libraries:  Boost, GLM, GL, GLEW,
#   stdc++, freeimageplus, SDL2, assimp, pthread.
#   Created by Edward C. Eberle <eberdeed@ebedeed.net>
#   March 2020 San Diego California USA
############################################################
cmake_minimum_required(VERSION 3.12)
if (POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()
project(astercube)
add_subdirectory(assimpopengl)
add_subdirectory(src)
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN YES)
set(PROJECT_NAME "Super Cube")
set(PROJECT_BRIEF "A program to display instanced blender objects in a sky box.")
doxygen_add_docs(doc include src assimpopengl/include assimpopengl/src COMMENT "Generating documentation.")
install(DIRECTORY openglresources DESTINATION /usr/share FILE_PERMISSIONS WORLD_READ)
install(DIRECTORY assimpopengl/LGPL DESTINATION /usr/share/doc/astercube-doc FILE_PERMISSIONS WORLD_READ)
install(DIRECTORY build/html DESTINATION /usr/share/doc/astercube-doc FILE_PERMISSIONS WORLD_READ)
install(DIRECTORY build/man DESTINATION /usr/share/doc/astercube-doc FILE_PERMISSIONS WORLD_READ)
install(FILES README.txt CHANGELOG.txt astercubewriteup.odt astercubewriteup.pdf
DESTINATION /usr/share/doc/astercube-doc PERMISSIONS WORLD_READ)
install(FILES build/src/astercube DESTINATION /usr/bin PERMISSIONS WORLD_READ WORLD_EXECUTE)
