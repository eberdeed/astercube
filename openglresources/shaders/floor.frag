/**********************************************************
 *   floor.frag:  A shader to render light refraction
 *   and reflection on an OpenGL object. It was found 
 *   on www.learnopengl.com.
 *   When the skybox requires a floor to it I use this.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   01/2020 San Diego, California USA
 *   Copyright Maureen A. Quinn 02/2020
 * ********************************************************/
 
// From www.learnopengl.com
#version 300 es
precision highp float;

in vec2 TexCoord;
out vec4 color;

uniform sampler2D ground;

vec2 floorvec;
void main()
{ 
    color = texture(ground, TexCoord);
    //color = vec4(0.0, 1.0, 0.0, 1.0);
}  
