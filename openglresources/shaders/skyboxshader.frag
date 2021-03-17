/**********************************************************
 *   skyboxshader.frag:  A shader to render a skybox. 
 *   It was found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   01/2020 San Diego, California USA
 * ********************************************************/
#version 300 es
precision highp float;

in vec3 textureDir; // Direction vector representing a 3D texture coordinate

out vec4 color;

uniform samplerCube skybox;  // Cubemap texture sampler

void main()
{             
    color = texture(skybox, textureDir);
}  
