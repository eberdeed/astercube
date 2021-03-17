/**********************************************************
 *   test2.vs:  A shader to render OpenGl objects.  This
 *   is the fundamental vertex shader. It was found 
 *   on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   01/2020 San Diego, California USA
 *   Copyright Maureen A. Quinn 02/2020
 * ********************************************************/

#version 300 es
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    gl_PointSize = 20.0;
} 
