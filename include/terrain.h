/**********************************************************
 *   Terrain:  A class to display a patch of ground inside
 *   a sky box.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   February 2021 San Diego, California USA
 * ********************************************************/

#ifndef TERRAIN_H
#define TERRAIN_H
#include "commonheader.h"
//! Using the assimpopengl library components.
#include <assimpopengl/createimage.h>
#include <assimpopengl/shader.h>
#include <assimpopengl/assimpopengl.h>

/** \class Terrain A patch of ground displayed inside a skybox.
 */
class Terrain
{
public:
    /** \brief Echo the instantiation of the class.
     */
    Terrain();
    /** \brief Delete the objects used.
     */
    ~Terrain();
    /** \brief Set the size of the square to display.
     */
    void setScale(float value);
    /** \brief Create the buffer and textures to be used.
     */
    void initTerrain();
    /** \brief Draw the surface.
     */
    void drawTerrain(mat4 model, mat4 view, mat4 projection);
    /** \brief Display the vertices of the matrix describing
     *  the square of the surface.
     */
    void debug();
    /** \brief Image handling class.
     */
    CreateImage *image;
    /** \brief Shader to display the surface.
     */
    Shader *floorShader;
    /** \brief Pointers to the sampler and buffers.
     */
    unsigned int floorTex, floorVAO, floorVBO;
    //! Affine matrix for rotating and scaling the surface.
    mat4 adjust = mat4(1.0f);
    //! The overall size.
    float size;
    //! The surface vertices.
    GLfloat floorVertices[30] = {
       -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
       -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
       -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f
    };
    //! Final matrix pointer.
    float *floorLayout;
    bool debug1 = false;
};

#endif // TERRAIN_H
