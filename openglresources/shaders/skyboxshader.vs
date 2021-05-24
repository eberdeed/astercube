/**********************************************************
 *   skyboxshader.vs:  A shader to render a skybox. 
 *   It was found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   01/2020 San Diego, California USA
 * ********************************************************/
#version 300 es
precision highp float;
layout (location = 0) in vec3 position;

out vec3 textureDir;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

vec4 tmpvec;

void main()
{
      
    gl_Position = projection * view * model * vec4(position, 1.0);
    tmpvec = model * vec4(position, 1.0);
    textureDir = tmpvec.xyz;
}
