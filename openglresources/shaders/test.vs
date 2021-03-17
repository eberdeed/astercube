/**********************************************************
 *   test.vs:  A shader to render the form
 *   of an instanced virtual object for an OpenGL program.  It 
 *   passes along position, normals, and texcoords
 *   and its original form was found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   March 2021 San Diego, California USA
 * ********************************************************/ 
/** \class test.vs
  *  Vertex shader.
  */
#version 300 es

#define NUM_INSTANCES 36

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

layout uniform itemData{
    mat4 location[NUM_INSTANCES];
};

vec4 tmpvec;

void main()
{
    //! Calculate the location of the vertex.
    tmpvec = projection * view * location[gl_InstanceID] * vec4(position, 1.0);
    gl_Position = tmpvec;
    locval.Position = tmpvec.xyz;
    //! Calculate the normal to the vertex.
    locval.Normal = vec4(location[gl_InstanceID] * vec4(normal, 1.0)).xyz;
    //! Pass along the texture coordinate.
    locval.TexCoord = texCoord;
    //gl_PointSize = 20.0;
} 
