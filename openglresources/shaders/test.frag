/**********************************************************
 *   test.frag:  A shader to render the colors
 *   for an OpenGL program.  It passes along reflection 
 *   data, and it was found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   03/2021 San Diego, California USA
 *   Copyright Maureen A. Quinn 03/2021
 * ********************************************************/ 

/** \class test.frag A shader to render the colors
 *   for an OpenGL program.  It passes along reflection 
 *   data, and it was found on www.learnopengl.com.
 */

#version 300 es

precision highp float;

struct Location
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
};

in Location locval;


out vec4 color;

uniform samplerCube SkyBoxOne;
uniform vec3 viewPos;
//! Texture (optional)
uniform sampler2D diffuseOne;
uniform sampler2D diffuseTwo;
uniform sampler2D specularOne;
uniform sampler2D binormalOne;
//! Specular intensity
uniform float shininess;
//! Toggle for texture
uniform int numDiffuse;
uniform bool isSpecular;
uniform bool diffOnly;
uniform bool isBiNormal;
//! Object transparency
uniform float opacity;
uniform float gamma;
uniform vec3 colordiff;
uniform int debug;
//! Shader variables
vec3 light, I;
vec4 texVal, texVal1, texVal2, lightVal;
vec3 lightDir, reflectDir, normal;
// diffuse shading
float diff;
// combine results
vec3 ambient;
vec3 diffuse;
vec3 tmpval;
float dotProd;
/** \brief Calculate the color based on light and camera
  *  position as well as surface normal.
  */
// function prototypes
float mydot(vec3 a, vec3 b);


void main()
{
    lightDir = normalize(vec3( -50.0,  150.0,  50.0) - locval.Position);
    if (numDiffuse == 1)
    {
        texVal = texture(diffuseOne, locval.TexCoord);
    }
    else if (numDiffuse == 2)
    {
        texVal1 = texture(diffuseOne, locval.TexCoord);
        texVal2 = texture(diffuseTwo, locval.TexCoord);
        texVal = mix(texVal1, texVal2, 0.5);
    }
    else
    {
        texVal = vec4(colordiff, opacity);
    }
    if (isBiNormal)
    {
        // obtain normal from normal map in range [0,1]
        normal = vec4(texture(binormalOne, locval.TexCoord)).xyz;
        // transform normal vector to range [-1,1]
        normal = (normal * 2.0) - 1.0;  // this normal is in tangent space
        normal = normalize(normal);
    }
    else
    {
        normal = locval.Normal;
    }
    //! \brief Vector describing direction from camera to object.
    I = normalize(viewPos - locval.Position);
    //! The reflected light.
    lightVal = texture(SkyBoxOne, I);
    //light = vec3(0.3, 0.3, 0.3);
    light = lightVal.xyz;
    // Diffuse shading
    dotProd = mydot(I, normal);
    diff = max(dotProd, 0.0);
    ambient  = 0.2 * texVal.xyz;
    diffuse  = light * diff * texVal.xyz;
    tmpval = ambient + diffuse;
    color = vec4(tmpval, 1.0);
    color = gamma * color ;
    color = texVal;
}  
/* Dot product */
float mydot(vec3 a, vec3 b)
{
    float total = 0.0;
    for (int x = 0; x < 3; x++)
    {
        total += a[x] * b[x];
    }
    return total;
}
        
