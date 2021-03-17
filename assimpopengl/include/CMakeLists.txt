############################################################
#   The CMake File for AssimpOpenGL library.  It provides for
#   adding the header files to the /usr/include/assimpopengl directory.
#   Created by Edward C. Eberle <eberdeed@ebedeed.net>
#   April 2020 San Diego California USA
############################################################
cmake_minimum_required(VERSION 3.12)
project(assimpopengl)
install(FILES assimpopengl.h createimage.h info.h mesh.h meshtex.h meshvert.h model.h shader.h 
commonheader.h camera.h uniformprinter.h DESTINATION /usr/include/assimpopengl PERMISSIONS WORLD_READ)
