/**********************************************************
 *   MeshTex:  A class that is derived from Mesh. This class
 *   is for textured meshes.  This class isolates tha
 *   display of data from a 3d asset file using assimp.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>,
 *   it was originally found on www.learnopengl.com.
 *   March 2020 San Diego, California USA
 * ********************************************************/

#ifndef MESHTEX_H
#define MESHTEX_H

#include "commonheader.h"
#include "mesh.h"
#include "shader.h"
#include "uniformprinter.h"
#include "assimpopengl.h"

//! Forward declarations so it can be used as a library.
struct Vertex;
struct Texture;
struct PointLight;
struct SpotLight;
struct Texture;
/** \class MeshTex A class that encapsulates creating and 
 * displaying of a textured mesh.
 */
class MeshTex : public Mesh //Make the base class public so the functions are accessible
{
public:
    //! \brief Empty constructor.
    MeshTex();
    //! \brief Deallocate buffers and arrays.
    ~MeshTex();
    /*  Functions  */
    /** \brief Provide the class with the data it need.
     * vertices : The vertex array.
     * indices : The index array.
     * textures : The texture vector.
     * vertSize : The number of vertices.
     * indexSize : The number of indices.
     * instanced : The instancing flag.
     * quantity : The number of instances.
     * shader : The shader.
     */
    int setData(Vertex *vertices, GLuint *indices, vector<Texture>textures, 
    int vertSize, int indexSize, bool instanced = false, int quantity = 1, 
    Shader *shader = nullptr, int startIndex = 0);
    /** \brief Pass in the data from the Model class to be realized here.
     *  camera.                                projection is the perspective for which I use the perspective funcition
     * of GLM in the camera class. 
     * model is the position and orientation of the mesh being drawn.
     * lights are point lights defined by the PointLight structure.
     * spotLights are spotlights defined by the SpotLight structure.
     * viewPos is the camera position.
     * startIndex is the current index of the textures being used.
     * diffOnly when set to true will provide an image without specular highlights.
     * gamma controls brightness of the textured mesh.
     */
    void Draw(glm::mat4 view, glm::mat4 projection, mat4 model, vector<PointLight> lights, 
    vector<SpotLight>spotLights, vec3 viewPos, bool diffOnly = true, float gamma = 1.0f);
    /** \brief A virtual function implemented and used by both classes to draw instanced data. The only 
     * difference from the above is that the model variable is a vector.  
     * view is the position and orientation of the camera.  
     * se the perspective funcition            of GLM in the camera class. 
     * model is the position and orientation of the mesh being drawn.
     * lights are point lights defined by the PointLight structure.
     * spotLights are spotlights defined by the SpotLight structure.
     * viewPos is the camera position.
     * startIndex is the current index of the textures being used.
     * diffOnly when set to true will provide an image without specular highlights.
     * gamma controls brightness of the textured mesh.
     */
    void DrawInstanced(mat4 view, mat4 projection, vector<mat4>model, 
    vector<PointLight> lights, vector<SpotLight>spotLights, vec3 viewPos, 
    bool diffOnly = true, float gamma = 1.0f);
    //! \brief For debugging.
    void dumpData();
    //! \brief Debug instance data.
    void debug(mat4 *modelData);
    //! \brief Create the mesh data as an OpenGL buffer object.
    void setupMesh();
    //! \brief Create the mesh data as an instanced OpenGL buffer object.
    void setupInstancedMesh();
    //! \brief For debugging:  Print a vector of three floats.
    void printVec3(vec3 vecVal);
    //! \brief For debugging:  Print a 4x4 matrix of floats.
    void printMat4(mat4 matVal);
    /* Variables */
    /*  Mesh Data  */
    //! Class global variables.
    //! The Vertex array.
    Vertex *vertices;
    //! The indexed Vertex array.
    Vertex *verticesIndexed;
    //! The index array.
    GLuint *indices;
    //! The associated textures as a vector.
    vector<Texture>textures;
    //! The instance location array.
    mat4 *instanceArray;
    /*  Render data  */
    //! The buffer object handles.
    GLuint VAO, VBO[2], EBO;
    //! The upper bounds of the various buffers.
    int vertSize, indexSize, texSize, total;
    //! Copious debug data.
    bool debug1 = false;
};


#endif // MESHTEX_H
