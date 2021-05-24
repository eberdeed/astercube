/* *******************************************************************
 * assimpopengl.h:  A file to provide an interface for the
 * assimpopengl library.
 * Created by:  Edward Charles Eberle <eberdeed@eberdeed.net>
 * April 2020 San Diego, California United States of America
 * *******************************************************************
 * ******************************************************************/
 /**  \class assimpopengl.h: A common header file for the assimpopengl
 *  library.  It provides a standard interface for all the classes
 *  involved in transforming the blender object into an OpenGL buffer object.
 *  Created by Edward C. Eberle <eberdeed@eberdeed.net>
 *  December 17, 2016, San Diego California USA
 *  Reworked April 17, 2020.
 */
#ifndef ASSIMPOPENGL_H
#define ASSIMPOPENGL_H
//! Particle lifetime for the SmokeCloud class.
#define LIFETIME 200
//! Assimp the Asset Importation Library
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Free Image Plus
#include <FreeImagePlus.h>

//! This library
#include "commonheader.h"
#include "model.h"
#include "mesh.h"
#include "info.h"
#include "createimage.h"
#include "shader.h"
#include "camera.h"
#include "uniformprinter.h"
#include "smokecloud.h"

/** \brief A structure to contain a vertex
 * for a textured mesh.
 */
struct Vertex {
    //! Vertex Postion cooridinates.
    float Position[3];
    //! Normal to the surface at the vertex point.
    float Normal[3];
    //! Texture coordinates.
    float TexCoords[2];
};
/** \brief A structure to contain a texture 
 * for display on the surface of a mesh.
 */
struct Texture {
    //! The handle to the OpenGL texture buffer.
    GLuint id;
    //! The type of the texture, for example "diffuse."
    string type;
    //! The file location of the image file defining the texture.
    string path;
};
/** \brief A structure to define a point light.
 */
struct PointLight {    
    //! The position of the light.
    vec3 position;
    //! The constant value of the light.
    float constant;
    //! The linear degredation value of the light.
    float linear;
    //! The quadratic degredation value of the light/
    float quadratic;  
    //! The ambient color value.
    vec3 ambient;
    //! The diffuse color value.
    vec3 diffuse;
    //! The specular color value.
    vec3 specular;
};
/** \brief A structure to define a spotlight.
 */
struct SpotLight {
    //! The  position of the light.
    vec3 position;
    //! The direction the spotlight points in.
    vec3 direction;
    //! The cut off angle in degrees.
    float cutOff;
    //! The outer cut off angle in degrees.
    float outerCutOff;
    //! The constant value of the light.
    float constant;
    //! The linear degredation value of the light.
    float linear;
    //! The quadratic degredation value of the light/
    float quadratic;  
    //! The ambient color value.
    vec3 ambient;
    //! The diffuse color value.
    vec3 diffuse;
    //! The specular color value.
    vec3 specular;
};



#endif // ASSIMPOPENGL_H
