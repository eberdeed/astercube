/*********************************************************************
 * *******************************************************************
 * commonheader:  A file to provide common includes.
 * Created by:  Edward Charles Eberle <eberdeed@eberdeed.net>
 * April 2020 San Diego, California United States of America
 * *******************************************************************
 * ******************************************************************/
/**  \class commonheader.h: A common header file for the AssimpOpenGL library.
 *  Created by Edward C. Eberle <eberdeed@eberdeed.net>
 *  December 17, 2016, San Diego California USA
 *  Reworked for OpenGL April 11, 2020
 */

/** \brief These are some files that we need to include to use OpenGL. 
 * Also included are GLEW, GLU, pthread, the OpenGL Math Library (GLM), the Boost 
 * library, the Assimp library, c++ system files, Free Image Plus, X11, OpenGL and SDL2.
 */
#ifndef COMMONHEADER_H
#define COMMONHEADER_H
//! The size of an exploded piece of the object (in triangles).
#define CHUNK_SIZE 10
// GLEW The OpenGL library manager
#define GLEW_STATIC
#include <glew.h>

// GLM The OpenGL Math Library
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

// Assimp the Asset Importation Library
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Free Image Plus
#include <FreeImagePlus.h>

// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <exception>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <random>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cmath>

// Boost The standard template library clone++.
#include <filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

// X11
#include <X11/Xlib.h>

// OpenGL
#include <GL/glext.h>

// SDL2
#include <SDL.h>

// The namespaces used.
using namespace std;
using namespace glm;
using namespace boost::filesystem;



#endif // COMMONHEADER_H
