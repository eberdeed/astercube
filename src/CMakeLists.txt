# ******************************************************************
# CMakeLists.txt:  A template for the building of a c++ program.
# Edward C. Eberle <eberdeed@eberdeed.net>
# December 2019 San Diego, California USA
# ****************************************************************/

cmake_minimum_required(VERSION 2.6)
project(astercube)
add_executable(astercube astercube.cpp asterobject.cpp skybox.cpp objects.cpp)
add_definitions(-g -fPIC -std=c++17 -pthread)
include_directories(/usr/include/glm /usr/include/GL
/usr/include/assimp /usr/include/boost /usr/include/SDL2)
link_directories(/usr/lib /usr/lib/x86_64-linux-gnu)
target_link_libraries(astercube stdc++ GL GLEW SDL2-2.0 
assimpopengl freeimage freeimageplus boost_filesystem pthread boost_system X11)
