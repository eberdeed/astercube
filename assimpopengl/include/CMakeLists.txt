############################################################
#   The CMake File for the includes in the AssimpOpenGL 
#   library.  It provides for adding the header files to 
#   the /usr/include/assimpopengl-1.0 directory.
#   Created by Edward C. Eberle <eberdeed@ebedeed.net>
#   April 2020 San Diego California USA
############################################################
cmake_minimum_required(VERSION 3.12)
project(assimpopengl)
install(FILES assimpopengl.h createimage.h info.h mesh.h model.h shader.h 
commonheader.h camera.h uniformprinter.h smokecloud.h DESTINATION 
/usr/include/assimpopengl-1.0 PERMISSIONS WORLD_READ)
