/**************************************************************
 * Objects:  A class to encapsulate the creation and use of 
 * the assimpopengl library for the display of instanced blender 
 * objects. Note that the total number of objects can be decided
 * by AMOUNT * QUANTITY, but that number has to be equal to
 * i * j * k in the createAsteroids function, otherwise objects
 * will be left without a defined location, which may break the
 * program.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * February 2021 San Diego, California USA
 ***********************************************************/


#ifndef OBJECTS_H
#define OBJECTS_H
#include "../assimpopengl/include/commonheader.h"
#include "../assimpopengl/include/assimpopengl.h"
#include "../assimpopengl/include/shader.h"
#include "asterobject.h"
#define QUANTITY 36
//! The number of asteroid models.
#define AMOUNT 6
struct PointLight;
struct SpotLight;

/** \class Objects A container to display blender objects
 * using the assimpopengl library.
 */
class Objects
{
public:
    /** \brief Set up the lighting and position data for the objects.
     */
    Objects();
    /** \brief Delete the pointers to the objects used.
     */
    ~Objects();
    /** \brief Instatiate the objects.
     */
    //! \brief AsterLoc is a structure used to contain asteroid data for field groups.
    struct AsterLoc{
        //! Completed location value as an Affine matrix.
        mat4 objMatrices;
        //! Location.
        vec3 objLocs;
        //! Radius or scale.
        float radii;
        //! Speed.
        vec3 velocity;
        //! Axis of rotation.
        vec3 axis;
        //! Total rotation.
        float rotation;
        //! Rotation velocity.
        float rotVel;
    };
    //! An array instance of AsterLoc to hold the asteroid field.
    AsterLoc tmpLocs[QUANTITY * AMOUNT];
    //! \brief assign location, size, velocity and rotation of an asteroid.
    void createAsteroids(int amount);
    //! \brief Print a three float vector.
    void printVec3(vec3 vecVal);
    //! \breif Print a 4x4 float matrix.
    void printMat4(mat4 matVal);
    //! \brief Initialize all the objects.
    void initObjects();
    //! \brief Calculate one's position.
    void calcPosition(mat4 model, int amount);
    //! \breif Blender model info.
    void debug();
    /** \brief Set the scale for the objects,
     *  assign the objects and calculate each
     *  object's position and orientation.
     */
    void setScale(float value);
    /** \brief Draw the objects on the  screen.
     */
    void drawObjects(mat4 model, mat4 view, mat4 projection, vec3 viewPos);
    /** \brief Get a normalized direction from two points.
     */
    vec3 getDirection(vec3 viewer, vec3 viewed);
    /** \brief Set the skybox the objects appear inside.
     */
    void setSkyBox(unsigned int skybox);
    /** \brief Create a random vector.
     */
    vec3 randVec();
    //! Constants for object manipulation.
    const float pi360 = acos(-1.0f) * 2.0f;  //! acos(-1) = pi.
    const float pi180 = acos(-1.0f);
    const float pi90 = acos(-1.0f) / 2.0f;
    //! The shader to display the objects.
    Shader *shader;
    //! The pointer to the skybox sampler cube.
    unsigned int skyboxTex;
    //! The asteroid location data isolated.
    float *instanceData;
    //! Affine matrix location for each instance of a particular asteroid.
    vector<mat4>modelData;
    //! The scale variable.
    int cubescale = 0;
    //! Dummy variables to satisfy libassimopengl. Used when needed.
    vector<PointLight>lights;
    vector<SpotLight>spotLights;
    //! Define a random float generator. 
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 generator; //Standard mersenne_twister_engine seeded with rd()    
    uniform_real_distribution<float>randomFloats; // generates random floats between 0.0 and 1.0

    //! The limits of the asteroid field.
    GLfloat maxx, maxy, maxz, minx, miny, minz;
    //! The current rotation of a given asteroid.
    float rotation;
    //! The OpenGL buffers.
    unsigned int VAO[2], VBO[3], dataIndex;
    //! Temporary.
    AsterLoc tmpLoc;
    //! Position variables.
    vec3 viewPos, transvec, velocity;
    //! Object pointer for creation and drawing in OpenGL.
    Model *figure;
     /** \brief A structure to contain the bulk of the
     *  configuration and mesh information for the given
     *  blender objects. See the info.h file for more information.
     */
    vector<ModelInfo>modelinfo;
    //! A single model definition value.
    ModelInfo item;
    //! The asteroid definiation file.  I used the WaveFront .obj format
    //! exported from blender.
    string asteroids[6] = {
        "/usr/share/openglresources/asteroids/asteroid1.obj",
        "/usr/share/openglresources/asteroids/asteroid2.obj",
        "/usr/share/openglresources/asteroids/asteroid3.obj",
        "/usr/share/openglresources/asteroids/asteroid4.obj",
        "/usr/share/openglresources/asteroids/asteroid5.obj",
        "/usr/share/openglresources/asteroids/asteroid6.obj"
    };
    //! Shader locations.
    string vertexShader = "/usr/share/openglresources/shaders/test.vs";
    string fragmentShader = "/usr/share/openglresources/shaders/test.frag";
    //! Debug flag.
    bool debug1 = false;
};

#endif // OBJECTS_H
