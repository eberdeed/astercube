/**********************************************************
 *   diffspecbinorm.vs:  A shader to render the form
 *   for an OpenGL program.  It passes along position,
 *   normals, binormal (tangents) and texcoords
 *   and its original form was found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   03/2020 San Diego, California USA
 * ********************************************************/ 
/** \class diffspecreflect.vs
  *  Vertex shader.
  */

#version 300 es
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

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    locval.Position = vec4(model * vec4(position, 1.0)).xyz;
    locval.Normal = vec4(model * vec4(normal, 1.0)).xyz;
    locval.TexCoord = texCoord;
} 
