/**********************************************************
 *   SmokeCloud:  A class to encapsulate creating an 
 *   gaseous cloud display with sound using SDL2 for sound
 *   and threading.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   03/2020 San Diego, California USA
 * ********************************************************/

#ifndef SMOKECLOUD_H
#define SMOKECLOUD_H
#define NUM_PARTICLES 10000
#define EXP_GAP 13000
#define SCALE 2
#include "assimpopengl.h"
#include "commonheader.h"
#include "shader.h"
#include "uniformprinter.h"
#include <iostream>
#include <vector>
#include <random>
#include <SDL.h>
#include <chrono>
using namespace std;
/** \class SmokeCloud Create as spinning cloud of particles.
 */
class SmokeCloud
{
public:
    /**
     * \brief Pass the shader during class instantiaton.
     */
    SmokeCloud(Shader *shader);

    /**
     * \brief Destroy the class.
     */
    ~SmokeCloud();
    /** \brief initFire defines the particles and their paths
     *  while also providing the lifespan of each particle.
     */
    void initFire();
    /** \brief defineObjects defines the OpenGL 
     *  buffer array used to draw the cloud.
     */
    void defineObjects();
    /** \brief debug provides the particle information.
     */
    void debug();
    /** \brief Pass an output stream and message for error
     * reporting.
     */
    void logSDLError(ostream &os, const string &msg);
    /** \brief Draw the cloud.
     * model : Cloud position and orientation.
     * view : Camera position and orientation.
     * projection : The Affine perspective matrix.
     * interval : The current frame being drawn.
     */
    void drawCloud(mat4 model, mat4 view, float interval);
    /** \brief Debug:  Print a vec3.
     */
    void printVec3(vec3 vecVal);
    /** \brief Debug:  Print a vec4.
     */
    void printVec4(vec4 vecVal);
    /** \brief Debug:  Print a mat4.
     */
    void printMat4(mat4 matVal);
    
    /* Variables */
    //! 180 degrees in radians.
    const float pi180 = acos(-1.0f);
    //! 360 degrees in radians.
    const float pi360 = acos(-1.0f) * 2.0f;
    //! Explosion color.
    vec4 color;
    //! Explosion scaler.
    mat4 scaler;
    //! Define a random float generator. 
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 generator; //Standard mersenne_twister_engine seeded with rd()    
    uniform_real_distribution<float>randomFloats; // generates random floats between 0.0 and 1.0    
    //! For the display of the gaseous cloud.
    Shader *shader;
    /** The array containing the particle data -- spherical coordinates.
     * particles[0] = phi
     * particles[1] = rho
     * particles[2] = radius
     * particles[3] = particle lifetime
     */
    vec4 particles[NUM_PARTICLES];
    //! The time point to seed the random number generator.
    chrono::system_clock::time_point tdata;
    //! The OpenGL buffer and vertex array handles.
    GLuint VAO = 0, VBO = 0;
    bool debug1 = false;
};
static SDL_AudioDeviceID dev;

#endif // SMOKECLOUD_H
