/**********************************************************
 *   test.frag:  A shader to render a constant color value. 
 *   It was found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   01/2020 San Diego, California USA
 *   Copyright Maureen A. Quinn 02/2020
 * ********************************************************/
 
// From www.learnopengl.com
#version 300 es
precision highp float;

out vec4 color;


void main()
{ 
    color = vec4(0.0, 1.0, 0.0, 1.0);
}  
