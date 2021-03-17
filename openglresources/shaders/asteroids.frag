/**********************************************************
 *   asteroid.frag:  A shader to render the colors
 *   for an OpenGL program.  It passes along reflection 
 *   data and lighting, and it was found on www.learnopengl.com.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   03/2020 San Diego, California USA
 *   Copyright Maureen A. Quinn 03/2020
 * ********************************************************/ 

/** \class asteroid.frag A shader to render the colors
 *   for an OpenGL program.  It passes along reflection 
 *   data, and it was found on www.learnopengl.com.
 */

#version 300 es

#define NR_POINT_LIGHTS 4  
#define NR_SPOT_LIGHTS 4

precision highp float;

struct Location
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
};

in Location locval;


out vec4 color;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
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
vec3 skyLight;
vec4 texVal, texVal1, texVal2, specVal, normal;
vec3 lightDir;
// diffuse shading
float diff;
// specular shading
vec3 reflectDir;
float spec;
// spotlight
float theta;
float epsilon;
float intensity;
// attenuation
float distance;
float attenuation;    
// combine results
vec3 ambient;
vec3 diffuse;
vec3 specular;
vec3 I;
vec3 R;
/** \brief Calculate the color based on light and camera
  *  position as well as surface normal.
  */
// function prototypes
vec3 CalcDirLight(vec3 light, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 viewDir);
void calcDebug(int flag, vec3 R, vec3 I, vec3 skyLight);
void main()
{

    if (numDiffuse == 1)
    {
        texVal = texture(diffuseOne, locval.TexCoord);
    }
    else if  (numDiffuse == 2)
    {
        texVal1 = texture(diffuseOne, locval.TexCoord);
        texVal2 = texture(diffuseTwo, locval.TexCoord);
        texVal = mix(texVal1, texVal2, 0.5);
    }
    else
    {
        texVal = vec4(colordiff, opacity);
    }
    if (isSpecular)
    {
        specVal = texture(specularOne, locval.TexCoord);
    }
    else
    {
        specVal = vec4(colordiff, opacity);
    }
    if (isBiNormal)
    {
        // obtain normal from normal map in range [0,1]
        normal = texture(binormalOne, locval.TexCoord);
        // transform normal vector to range [-1,1]
        normal = (normal * 2.0) - 1.0;  // this normal is in tangent space
        normal = normalize(normal);
    }
    else
    {
        normal = normalize(vec4(locval.Normal, 1.0));
    }
    //! \brief Vector describing direction from camera to object.
    I = normalize(viewPos - locval.Position);
    R = reflect(I, normal.xyz);
    skyLight = vec4(texture(SkyBoxOne, I)).xyz;
    if (debug == 0)
    {
        color = vec4(CalcDirLight(skyLight, R), opacity);
        for (int x = 0; x < NR_SPOT_LIGHTS; x++)
        {
            color += vec4(CalcSpotLight(spotLights[x], I), opacity);
        }
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
        {
            color += vec4(CalcPointLight(pointLights[i], I), opacity);
        }
    }
    else
    {
        calcDebug(debug, R, I, skyLight);
    }
    color = gamma * color ;
}  
// calculates the color when using a directional light.
vec3 CalcDirLight(vec3 light, vec3 viewDir)
{
    lightDir = normalize(vec3( -50.0,  150.0,  50.0) - locval.Position);
    // Diffuse shading
    diff = max(dot(lightDir, normal.xyz), 0.0);
    //diff = 1.0;
    // Specular shading
    reflectDir = reflect(-lightDir, normal.xyz);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    //spec = 1.0;
    // Combine results
    
    ambient  = 0.2 * texVal.xyz;
    diffuse  = light * diff * texVal.xyz;
    specular = light * spec * specVal.xyz;
    if (diffOnly)
    {
        return (ambient + diffuse);
    }
    else
    {
        return vec3(ambient + diffuse + specular);
    }
} 

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 viewDir)
{
    lightDir = normalize(light.position - locval.Position);
    // diffuse shading
    diff = max(dot(lightDir, normal.xyz), 0.0);
    // specular shading
    reflectDir = reflect(-lightDir, normal.xyz);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    //spec = 1.0;
    // attenuation
    distance = length(light.position - locval.Position);
    attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));    
    // combine results
    ambient = light.ambient * texVal.xyz;
    diffuse = light.diffuse * diff * texVal.xyz;
    specular = light.specular * spec * specVal.xyz;
    //attenuation = 0.007936507936507936;
    ambient = ambient * attenuation;
    diffuse = diffuse * attenuation;
    specular = specular * attenuation;
    //vec3 specular = vec3(0.0, 0.0, 0.0);
    if (diffOnly)
    {
        return (ambient + diffuse);
    }
    else
    {
        //return (ambient + diffuse + specular);
        return diffuse;
    }
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 viewDir)
{
    lightDir = normalize(light.position - locval.Position);
    // diffuse shading
    diff = max(dot(light.direction, normal.xyz), 0.0);
    // specular shading
    reflectDir = reflect(-light.direction, normal.xyz);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    distance = length(light.position - locval.Position);
    attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    theta = dot(lightDir, normalize(-light.direction)); 
    epsilon = light.cutOff - light.outerCutOff;
    intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //intensity = 2.0;
    //attenuation = 1.0;
    // combine results
    ambient = light.ambient * texVal.xyz;
    diffuse = light.diffuse * diff * texVal.xyz;
    specular = light.specular * spec * specVal.xyz;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    if (diffOnly)
    {
        return (ambient + diffuse);
    }
    else
    {
        return vec3(ambient + diffuse + specular);
    }
}

void calcDebug(int flag, vec3 R, vec3 I, vec3 skyLight)
{
    switch(flag)
    {
        case 1:
            color = vec4(CalcDirLight(skyLight, R), opacity);
            break;
        case 2:
            color = vec4(CalcSpotLight(spotLights[0], I), opacity);
            for (int x = 1; x < NR_SPOT_LIGHTS; x++)
            {
                color += vec4(CalcSpotLight(spotLights[x], I), opacity);
            }
            break;
        case 3:
            color = vec4(CalcPointLight(pointLights[0], I), opacity);
            for(int i = 1; i < NR_POINT_LIGHTS; i++)
            {
                color += vec4(CalcPointLight(pointLights[i], I), opacity);
            }
        default:
            color = vec4(CalcDirLight(skyLight, R), opacity);
            for (int x = 0; x < NR_SPOT_LIGHTS; x++)
            {
                //color += vec4(CalcSpotLight(spotLights[x], I), opacity);
            }
            for(int i = 0; i < NR_POINT_LIGHTS; i++)
            {
                //color += vec4(CalcPointLight(pointLights[i], I), opacity);
            }
    }
}
