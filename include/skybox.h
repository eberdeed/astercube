/**************************************************************
 * SkyBox:  A class to encapsulate the creation and use of 
 * a set of a sky box images.  
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * August 2020 San Diego, California USA
 ***********************************************************/


#ifndef SKYBOX_H
#define SKYBOX_H
#include "commonheader.h"
//! Using components from the assimpopengl library.
#include "../assimpopengl/include/createimage.h"
#include "../assimpopengl/include/shader.h"
#include "asterobject.h"

/** \class SkyBox  Display a skybox for a given scene.
 */
class SkyBox
{
public:
    /** \brief Echo the instantiation of the skybox.
     */
    SkyBox();
    /** \brief Echo the destruction of the skybox 
     *  and delete the objects used.
     */
    ~SkyBox();
    /** \brief Create the vertex array and assign
     * it to an OpenGl buffer as well as creating
     * the sampler cube.
     */
    void initSkyBox();
    /** \brief Set the overall size of the skybox.
     */
    void setScale(float value);
    /** \brief Draw the skybox.
     */
    void drawSkyBox(mat4 model, mat4 view, mat4 projection);
    /** \brief display the vertex values for the skybox.
     */
    void debug();
    /** \brief return the pointer to the sky box cube sampler.
     */
    unsigned int getSkyBox();
    //! Pointer to the image handling class.
    CreateImage *image;
    //! Pointer to the cube creating class.
    AsterObject *cuby;
    //! Pointer to the shader.
    Shader *skyboxShader;
    //! Pointer to the cube matrix.
    float *skycube;
    //! Overall size.
    float size;
    //! Texture and buffer pointers.
    unsigned int skyboxTex, skyboxVAO, skyboxVBO;
    //! Sky box images. Change these to change the skybox.
    string skybox[6] =
    {
        "/usr/share/openglresources/skyboxes/mp_drakeq/drakeq_rt.tga",
        "/usr/share/openglresources/skyboxes/mp_drakeq/drakeq_lf.tga",
        "/usr/share/openglresources/skyboxes/mp_drakeq/drakeq_up.tga",
        "/usr/share/openglresources/skyboxes/mp_drakeq/drakeq_dn.tga",
        "/usr/share/openglresources/skyboxes/mp_drakeq/drakeq_ft.tga",
        "/usr/share/openglresources/skyboxes/mp_drakeq/drakeq_bk.tga"
    };
    //! Shader locations.
    string vertexShader = "/usr/share/openglresources/shaders/skyboxshader.vs";
    string fragmentShader = "/usr/share/openglresources/shaders/skyboxshader.frag";
};

#endif // SKYBOX_H
