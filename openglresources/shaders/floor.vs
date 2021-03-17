/**********************************************************
 *   floor.vs:  A shader to render OpenGl objects.  This
 *   is the fundamental vertex shader. It was found 
 *   on www.learnopengl.com.
 *   When the skybox requires a floor to it I use this.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   01/2020 San Diego, California USA
 *   Copyright Maureen A. Quinn 02/2020
 * ********************************************************/
#version 300 es
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoord = texcoord;
} 
