/**********************************************************
 *   asteroid.vs:  A shader to render the instanced form
 *   of a virtual object for an OpenGL program.  It 
 *   passes along position, normals, and texcoords and 
 *   lighting. Its original form was found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   April 2020 San Diego, California USA
 * ********************************************************/ 
/** \class asteroid.vs
  *  Vertex shader.
  */
#version 300 es

#define NUM_INSTANCES 8

precision highp float;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

struct Location
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
};

out Location locval;

uniform mat4 view;
uniform mat4 projection;

layout (packed) uniform itemData{
    mat4 location[NUM_INSTANCES];
};

vec4 tmpvec;

void main()
{
    tmpvec = projection * view * location[gl_InstanceID] * vec4(position, 1.0);
    gl_Position = tmpvec;
    locval.Position = tmpvec.xyz;
    tmpvec = location[gl_InstanceID] * vec4(normal, 1.0);
    locval.Normal = tmpvec.xyz;
    locval.TexCoord = texCoord;
} 
