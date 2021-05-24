/**********************************************************
 *   explode.frag:  A shader to render a mutating color value. 
 *   It was adapted from a shader found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   03/2021 San Diego, California USA
 *   Copyright Maureen A. Quinn 03/2021
 * ********************************************************/
 
// From www.learnopengl.com
#version 300 es
precision highp float;

uniform float timeVal;

out vec4 color;

float timeVal1;
void main()
{ 
    //! if the time value is greater that 200 make the color white.
    if (timeVal > 200.0)
    {
       color = vec4(1.0, 1.0, 1.0, 1.0);
       return;
    }
    //! if the time value is greater that 100.0 set green to the maximum (1.0).
    timeVal1 = timeVal;
    if (timeVal1 > 100.0)
    {
        timeVal1 = 100.0;
    }
    //! Make green and blue increase their influence over time.
    color = vec4(1.0, (1.0/ 100.0) * timeVal1, (1.0/ 200.0) * timeVal, 1.0);
}  
