/**********************************************************
 *   MeshVert:  A class that is derived from Mesh. This class
 *   is for untextured meshes.  This class isolates the
 *   display of data from a 3d asset file using assimp.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   and originally found on www.learnopengl.com.
 *   March 2020 San Diego, California USA
 * ********************************************************/

#ifndef MESHVERT_H
#define MESHVERT_H

#include "commonheader.h"
#include "mesh.h"
#include "shader.h"
#include "assimpopengl.h"

//! Forward declarations so it can be used as a library.
struct Vertex1;
struct PointLight;
struct SpotLight;

/** \class MeshVert A class to encapsulate the 
 *  display of 3D assets acquired by the Model 
 *  class using the Assimp library.  This 
 *  class is for meshes that do not contain any 
 *  textures.
 */
class MeshVert : public Mesh
{
public:
    //! \brief Empty constructor.
    MeshVert();
    //! \brief Destructor destroying a vertex array and an index buffer.
    ~MeshVert();
    /* Functions */
    /**  \brief Pass the data necessary to display a three dimension object with no textures.
     * vertices : The vertex array.
     * indices : The index array.
     * vertSize : The number of vertices.
     * indexSize : The number of indices.
     * instanced : The instancing flag.
     * quantity : The number of instances.
     * shader : The shader.
     */
    int setData(Vertex1 *vertices, GLuint *indices, vec3 color, int vertSize, int indexSize, bool instanced = false, int quantity = 1, Shader *shader = nullptr, int startIndex = 0);
    /** \brief Pass data to be displayed here from the Model class.
     * view is the position and orientation of the camera.  
     * projection is the perspective for which I use the perspective funcition
     * of GLM in the camera class. 
     * model is the position and orientation of the mesh being drawn.
     * lights are point lights defined by the PointLight structure.
     * spotLights are spotlights defined by the SpotLight structure.
     * viewPos is the camera position.
     * startIndex is the current index of the textures being used.
     * diffOnly when set to true will provide an image without specular highlights.
     * gamma controls brightness of the textured mesh.
     */
    void Draw(glm::mat4 view, glm::mat4 projection, glm::mat4 model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, bool diffOnly = true, float gamma = 1.0f);
    /** \brief A virtual function implemented and used by both classes to draw instanced data. The only 
     * difference from the above is that the model variable is a vector.  
     * view is the position and orientation of the camera.  
     * projection is the perspective for which I use the perspective funcition
     * of GLM in the camera class. 
     * model is the position and orientation of the mesh being drawn.
     * lights are point lights defined by the PointLight structure.
     * spotLights are spotlights defined by the SpotLight structure.
     * viewPos is the camera position.
     * startIndex is the current index of the textures being used.
     * diffOnly when set to true will provide an image without specular highlights.
     * gamma controls brightness of the textured mesh.
     */
    void DrawInstanced(mat4 view, mat4 projection, vector<mat4>model, vector<PointLight> lights, vector<SpotLight>spotLights, vec3 viewPos, bool diffOnly = true, float gamma = 1.0f);
    //! Class global variables.
    /* Variables */
    /*  Mesh Data  */
    //! The vertex array.
    Vertex1 *vertices;
    //! The index array.
    GLuint *indices;
    //! \brief Debugging function.
    void dumpData();
    /*  Render data  */
    //! The OpenGL buffer object handles.
    GLuint VAO, VBO[2], EBO;
    //! The instance data location.
    mat4 *modelData;
    //! The upper bounds of the vertex and index arrays respectively.
    int vertSize, indexSize;
    //! The default color of the object.
    vec3 colordiff = vec3(1.0f, 1.0f, 1.0f);
    /*  Functions    */
    //! \brief Create the vertex array buffer, buffer object and index buffers.
    void setupMesh();
    //! Debug flag.
    bool debug1 = false;
};


#endif // MESHVERT_H
