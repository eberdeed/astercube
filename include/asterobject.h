/**************************************************************
 * AsterObject:  A class that creates a cube in a variety 
 * of ways.
 * Create 08/2020 by Edward C. Eberle <eberdeed@eberdeed.net>
 * San Diego, California USA
 *************************************************************/

#ifndef ASTEROBJECT_H
#define ASTEROBJECT_H
#define NUM_VERTICES 36
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
using namespace std;
using namespace glm;

/** \class AsterObject A class that will allow the creation of a cube as a
 *  single GLvoid* blob or as a series of GLvoid* blobs with the vertices,
 *  texture coordinates, and normals as seperate arrays.
 */
class AsterObject
{
public:
    /** \brief The default constructor generates the default matrices containing the basic
     *  cube data.
     */
    AsterObject();

    /** \brief Destructor, echoing the destruction of the class.
     */
    ~AsterObject();
    /** \brief Generates a specific type of cube based on the boolean
     *  flags provided.  The scale is the size of the cube.  Colored, allows
     *  for vertex coloring.  Normaled and textured allow for normal and texture
     *  coordinates.
     */
    float *genCube(float scale, bool colored, bool normaled, bool textured);
    /** \brief Generates the basic matrix values for a cube.
     */
    void genMatrices();
    /** \brief Creates the texture coordinates.
     */
    vec2 genTexture(vec3 normal, vec3 triangle);
    /** \brief Returns an unindexed matrix of cube
     * vertices.
     */
    float *getCube();
    /** \brief Returns a matrix of cube vertex colors.
     */
    float *getColor();
    /** \brief Returns a matrix of cube normals.
     */
    float *getNormals();
    /** \brief Returns a matrix of texture coordinates.
     */
    float *getTexCoords();
    /** \brief Returns a matrix of indices for an indexed cube.
     *  To get the values for an indexed cube's vertices, normals
     *  and texture coordinates access the martrices below directly.
     */
    unsigned int *getIndices();
    /** \brief Create an irregular cube to be a prototype asteroid.
     */
    float *genAsteroid();
    int asteroidSize();
    //! Pointer for the GLvoid blob value.
    float *cubeptr = nullptr;
    //! The calculated information for the cube.
    float calcTex[72];
    float calcColor[108];
    float calcNorm[108];
    float calcCube[108];
    //!---------------------------------------------------------
    //! These are the indexed matrix values.
    //!---------------------------------------------------------
            // Vertex        //Color
    vec3 cube[8] = {
    vec3(0.5f,  0.5f,  0.5f), //0
    vec3(-0.5f,  0.5f,  0.5f), //1
    vec3(-0.5f, -0.5f,  0.5f), //2
    vec3(0.5f, -0.5f,  0.5f), //3
    vec3(0.5f, -0.5f, -0.5f), //4
    vec3(0.5f,  0.5f, -0.5f), //5
    vec3(-0.5f,  0.5f, -0.5f), //6
    vec3(-0.5f, -0.5f, -0.5f)  //7
    };
    /** To do
     * Create randomly mishapen 
     * asteroids from a fixed number of
     * vertices.
     */
    vec3 smallasteroid[26] = {
        vec3(0.5f, 0.5f, 0.5f), // 1
        vec3(0.0f, 0.5f, 0.5f),
        vec3(-0.5, 0.5f, 0.5f),

        vec3(0.5f, 0.0f, 0.5f), // 2
        vec3(0.0f, 0.0f, 0.5f),
        vec3(-0.5, 0.0f, 0.5f),
        
        vec3(0.5f, -0.5f, 0.5f), // 3
        vec3(0.0f, -0.5f, 0.5f),
        vec3(-0.5, -0.5f, 0.5f),

        vec3(0.5f, 0.5f, -0.5f), // 4
        vec3(0.0f, 0.5f, -0.5f),
        vec3(-0.5, 0.5f, -0.5f),

        vec3(0.5f, 0.0f, -0.5f), // 5
        vec3(0.0f, 0.0f, -0.5f),
        vec3(-0.5, 0.0f, -0.5f),
        
        vec3(0.5f, -0.5f, -0.5f), // 6
        vec3(0.0f, -0.5f, -0.5f),
        vec3(-0.5, -0.5f, -0.5f),

        vec3(0.5, 0.5f, 0.0f), // 7
        vec3(0.5, 0.0f, 0.0f),
        vec3(0.5, -0.5f, 0.0f),
        
        vec3(-0.5, 0.5f, 0.0f), // 8
        vec3(-0.5, 0.0f, 0.0f),
        vec3(-0.5, -0.5f, 0.0f),
        
        vec3(0.0f, 0.5f, 0.0f), // 1
        vec3(0.0f, -0.5f, 0.0f),
    };
    /** To do
     * Create randomly mishapen 
     * asteroids from a fixed number of
     * vertices.
     */
    vec3 bigasteroid[107] = {
        
        vec3(1.0f, 1.0f, 1.0f), //1
        vec3(0.5f, 1.0f, 1.0f),
        vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f, 1.0f, 1.0f),
        vec3(-1.0f, 1.0f, 1.0f),
        
        vec3(1.0f, 0.5f, 1.0f), //2
        vec3(0.5f, 0.5f, 1.0f),
        vec3(0.0f, 0.5f, 1.0f),
        vec3(-0.5f, 0.5f, 1.0f),
        vec3(-1.0f, 0.5f, 1.0f),

        vec3(1.0f, 0.0f, 1.0f), //3
        vec3(0.5f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.0f, 1.0f),
        vec3(-1.0f, 0.0f, 1.0f),
        
        vec3(1.0f, -0.5f, 1.0f), //4
        vec3(0.5f, -0.5f, 1.0f),
        vec3(0.0f, -0.5f, 1.0f),
        vec3(-0.5f, -0.5f, 1.0f),
        vec3(-1.0f, -0.5f, 1.0f),
        
        vec3(1.0f, -1.0f, 1.0f), //5
        vec3(0.5f, -1.0f, 1.0f), 
        vec3(0.0f, -1.0f, 1.0f),
        vec3(-0.5f, -1.0f, 1.0f),
        vec3(-1.0f, -1.0f, 1.0f),

        vec3(1.0f, 1.0f, -1.0f), // 6
        vec3(0.5f, 1.0f, -1.0f),
        vec3(0.0f, 1.0f, -1.0f),
        vec3(-0.5f, 1.0f, -1.0f),
        vec3(-1.0f, 1.0f, -1.0f),
        
        vec3(1.0f, 0.5f, -1.0f), // 7
        vec3(0.5f, 0.5f, -1.0f),
        vec3(0.0f, 0.5f, -1.0f),
        vec3(-0.5f, 0.5f, -1.0f),
        vec3(-1.0f, 0.5f, -1.0f),

        vec3(1.0f, 0.0f, -1.0f), // 8
        vec3(0.5f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(-0.5f, 0.0f, -1.0f),
        vec3(-1.0f, 0.0f, -1.0f),
        
        vec3(1.0f, -0.5f, -1.0f), // 9
        vec3(0.5f, -0.5f, -1.0f),
        vec3(0.0f, -0.5f, -1.0f),
        vec3(-0.5f, -0.5f, -1.0f),
        vec3(-1.0f, -0.5f, -1.0f),
        
        vec3(1.0f, -1.0f, -1.0f), // 10
        vec3(0.5f, -1.0f, -1.0f),
        vec3(0.0f, -1.0f, -1.0f),
        vec3(-0.5f, -1.0f, -1.0f),
        vec3(-1.0f, -1.0f, -1.0f),
        
        vec3(1.0f, 1.0f, 0.5f), // 1 
        vec3(1.0f, 1.0f, 0.0f),
        vec3(1.0f, 1.0f, -0.5f),
        
        vec3(1.0f, 0.5f, 0.5f), // 2
        vec3(1.0f, 0.5f, 0.0f),
        vec3(1.0f, 0.5f, -0.5f),

        vec3(1.0f, 0.0f, 0.5f), // 3
        vec3(1.0f, 0.0f, 0.0f),
        vec3(1.0f, 0.0f, -0.5f),

        vec3(1.0f, -0.5f, 0.5f), // 4
        vec3(1.0f, -0.5f, 0.0f),
        vec3(1.0f, -0.5f, -0.5f),

        vec3(1.0f, -1.0f, 0.5f), // 5
        vec3(1.0f, -1.0f, 0.0f),
        vec3(1.0f, -1.0f, -0.5f),
       
        vec3(-1.0f, 1.0f, 0.5f), // 6
        vec3(-1.0f, 1.0f, 0.0f),
        vec3(-1.0f, 1.0f, -0.5f),
        
        vec3(-1.0f, 0.5f, 0.5f), // 7
        vec3(-1.0f, 0.5f, 0.0f),
        vec3(-1.0f, 0.5f, -0.5f),

        vec3(-1.0f, 0.0f, 0.5f), // 8
        vec3(-1.0f, 0.0f, 0.0f),
        vec3(-1.0f, 0.0f, -0.5f),

        vec3(-1.0f, -0.5f, 0.5f), // 9
        vec3(-1.0f, -0.5f, 0.0f),
        vec3(-1.0f, -0.5f, -0.5f),

        vec3(-1.0f, -1.0f, 0.5f), // 10
        vec3(-1.0f, -1.0f, 0.0f),
        vec3(-1.0f, -1.0f, -0.5f),
        
        vec3(0.5f, 1.0f, 0.5f), // 11
        vec3(0.0f, 1.0f, 0.5f),
        vec3(-0.5f, 1.0f, 0.5f),

        vec3(0.5f, 1.0f, 0.0f), // 12
        vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 1.0f, 0.0f),
 
        vec3(0.5f, 1.0f, -0.5f), // 13
        vec3(0.0f, 1.0f, -0.5f),
        vec3(-0.5f, 1.0f, -0.5f),

        vec3(0.5f, -1.0f, 0.5f), // 14
        vec3(0.0f, -1.0f, 0.5f),
        vec3(-0.5f, -1.0f, 0.5f),

        vec3(0.5f, -1.0f, 0.0f), //15
        vec3(0.0f, -1.0f, 0.0f),
        vec3(-0.5f, -1.0f, 0.0f),

        vec3(0.5f, -1.0f, -0.5f), // 16
        vec3(0.0f, -1.0f, -0.5f),
        vec3(-0.5f, -1.0f, -0.5f),

        vec3(1.0f, 0.0f, 0.0f), // 17
        vec3(0.0f, 1.0f, 0.0f), 
        vec3(0.0f, 0.0f, 1.0f),
        
        vec3(-1.0f, 0.0f, 0.0f), // 18
        vec3(0.0f, -1.0f, 0.0f), 
        vec3(0.0f, 0.0f, -1.0f), 
        
        vec3(1.0f, 0.0f, 0.0f), // 19
        vec3(0.0f, 1.0f, 0.0f), 
        vec3(0.0f, 0.0f, 1.0f)
    };
        
    vec3 normals[6] = {
         vec3( 1.0f,   0.0f,  0.0f),
         vec3( 0.0f,   1.0f,  0.0f),
         vec3( 0.0f,   0.0f,  1.0f),
         vec3(-1.0f,   0.0f,  0.0f),
         vec3( 0.0f,  -1.0f,  0.0f),
         vec3( 0.0f,   0.0f, -1.0f)
    };
    vec3 colors[8] = {
         vec3( 1.0f,   0.0f,  0.0f),
         vec3( 0.0f,   1.0f,  0.0f),
         vec3( 0.0f,   0.0f,  1.0f),
         vec3( 1.0f,   1.0f,  0.0f),
         vec3( 0.0f,   1.0f,  1.0f),
         vec3( 1.0f,   0.5f,  0.5f),
         vec3( 0.5f,   0.5f,  1.0f),
         vec3( 1.0f,   0.0f,  1.0f)
    };
    //! Note that 72 / 3 = 24
    float cubeVerts[72] =
    {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
    };

    //! Note that 72 / 3 = 24
    float cubeNormals[72] =
    {
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };

    //! Note that 48 / 2 = 24.
    float cubeTex[48] =
    {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };
    //! Note that 72 / 3 = 24.
    float cubeColors[72] = {
         1.0f,   0.0f,  0.0f,
         0.0f,   1.0f,  0.0f,
         0.0f,   0.0f,  1.0f,
         1.0f,   1.0f,  0.0f,
         0.0f,   1.0f,  1.0f,
         1.0f,   0.5f,  0.5f,
         0.5f,   0.5f,  1.0f,
         1.0f,   0.0f,  1.0f,
         1.0f,   0.0f,  0.0f,
         0.0f,   1.0f,  0.0f,
         0.0f,   0.0f,  1.0f,
         1.0f,   1.0f,  0.0f,
         0.0f,   1.0f,  1.0f,
         1.0f,   0.5f,  0.5f,
         0.5f,   0.5f,  1.0f,
         1.0f,   0.0f,  1.0f,
         1.0f,   0.0f,  0.0f,
         0.0f,   1.0f,  0.0f,
         0.0f,   0.0f,  1.0f,
         1.0f,   1.0f,  0.0f,
         0.0f,   1.0f,  1.0f,
         1.0f,   0.5f,  0.5f,
         0.5f,   0.5f,  1.0f,
         1.0f,   0.0f,  1.0f
    };
    //! These are the indices for the matrices above.
    //! There are 36 values for the 36 Vertices of the cube.
    //! If you assign the above matrix elements in the
    //! order given below you will have an indexed cube.
    unsigned int cubeIndices[NUM_VERTICES] =
    {
        0, 2, 1,
        0, 3, 2,
        4, 5, 6,
        4, 6, 7,
        8, 9, 10,
        8, 10, 11,
        12, 15, 14,
        12, 14, 13,
        16, 17, 18,
        16, 18, 19,
        20, 23, 22,
        20, 22, 21
    };
    
    /** This array shows the order of processing
     * for each vertex in the cube array.  The cube 
     * array defines each corner of the cube.  The
     * indices array allows the genMatrices function 
     * to generate a cube from the eight vertices. 
     * Each face of the cube has two triangles
     * or six vertices and there are six faces.
     * This is seperate from the indexed cube
     * because the indexed cube has normals which
     * overlap in some directions.
     */
    unsigned int indices[NUM_VERTICES] = {
        0, 1, 2, 0, 2, 3,
        0, 3, 4, 0, 4, 5,
        0, 5, 6, 0, 6, 1,
        7, 1, 6, 7, 2, 1,
        7, 5, 4, 7, 6, 5,
        7, 3, 2, 7, 4, 3
    };
    /** This is a "To do" item.  I
     * would like to generate randomly
     * mishapen asteroids using a fixed
     * number of vertices.
     */
    static const int smalltotal = 12;
    unsigned int smallindices[smalltotal] = {
       0, 1, 4,
       4, 0, 3,
       7, 4, 8,
       8, 4, 5
    };
};

#endif // ASTEROBJECT_H
