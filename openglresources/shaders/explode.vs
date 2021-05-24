/**********************************************************
 *   explode.vs:  A shader to render the explosion of an 
 *   OpenGl object.  It was found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   03/2021 San Diego, California USA
 *   Copyright Maureen A. Quinn 03/2021
 * ********************************************************/

#version 300 es
#define NUM_CHUNKS 500

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//! Index of the current chunk.
uniform int index;

//! chunk position data.
uniform itemData{
    mat4 location[NUM_CHUNKS];
};


void main()
{
    gl_Position = model * location[index] * projection * view  * vec4(position, 1.0f);
} 
