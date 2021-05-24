/*******************************************************************
 * CommonHeader:  A header file to reduce repetition of #define
 * and #include statements.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * January 2020 San Diego, California USA
 * Copyright Maureen A. Quinn 02/2020
****************************************************************/
/** \class CommonHeader A header file to reduce repetition of #define
 * and #include statements.
 */
#ifndef COMMONHEADER_H
#define COMMONHEADER_H
//! Limit the number of images in an image array. (Not Used)
#define NUM_IMAGES 16

//! GLEW The OpenGL library manager
#define GLEW_STATIC
#include <glew.h>

//! GLM The OpenGL math library
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/matrix_interpolation.hpp>
#include <gtc/matrix_access.hpp> 
#include <gtx/euler_angles.hpp>

//! SDL2
#include <SDL.h>

//! Assimp the Asset Importation Library
#include <assimpopengl-1.0/assimpopengl.h>

// X11
#include <X11/Xlib.h>

//! Std C++
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <cmath>
#include <exception>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>

//! Free Image Plus
#include <FreeImagePlus.h>
#include <FreeImage.h>

//! The namespaces used.
using namespace std;
using namespace glm;



#endif // COMMONHEADER_H
