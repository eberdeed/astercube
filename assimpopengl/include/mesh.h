/**********************************************************
 *   Mesh:  This class isolates the  display of data from 
 *   a 3d asset file using assimp.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>,
 *   it was originally found on www.learnopengl.com.
 *   March 2020 San Diego, California USA
 * ********************************************************/

#ifndef MESH_H
#define MESH_H

#include "commonheader.h"
#include "shader.h"
#include "uniformprinter.h"
#include "assimpopengl.h"
#include <vector>
#include <iostream>
using namespace std;
//! Forward declarations so it can be used as a library.
struct Vertex;
struct Texture;
struct PointLight;
struct SpotLight;
struct Texture;
class SmokeCloud;
/** \class Mesh A class that encapsulates creating and 
 * displaying of a mesh.  It can produce both textured
 * and untextured meshes and provides for blowing up
 * the object.
 */
class Mesh
{
public:
    //! \brief Empty constructor.
    Mesh();
    //! \brief Deallocate buffers and arrays.
    ~Mesh();
    /*  Functions  */
    /** \brief Provide the class with the data it need.
     * vertices : The vertex array.
     * indices : The index array.
     * textures : The texture vector.
     * colordiff : The untextured color.
     * vertSize : The number of vertices.
     * indexSize : The number of indices.
     * instanced : The instancing flag.
     * quantity : The number of instances.
     * shader : The shader.
     * explodeShader the shader for the explosion.
     * cloudShader the shader for the gaseous cloud during
     * the explosion.
     * startIndex : The first available texture buffer.
     */
    int setData(Vertex *vertices, GLuint *indices, vector<Texture>textures, 
    vec3 colordiff, int vertSize, int indexSize, bool instanced, int quantity, 
    Shader *shader, Shader *explodeShader, int startIndex);
    /** \brief Pass in the data from the Model class to be realized here.
     * view is the position and orientation of the camera.  
     * projection is the perspective for which I use the perspective function
     * of GLM in the camera class. 
     * model is the position and orientation of the mesh being drawn.
     * lights are point lights defined by the PointLight structure.
     * spotLights are spotlights defined by the SpotLight structure.
     * viewPos is the camera position.
     * diffOnly when set to true will provide an image without specular highlights.
     * gamma controls brightness of the textured mesh.
     */
    void Draw(glm::mat4 view, glm::mat4 projection, mat4 model, vector<PointLight> lights, 
    vector<SpotLight>spotLights, vec3 viewPos, bool diffOnly = true, float gamma = 1.0f);
    /** \brief A virtual function implemented and used by both classes to draw 
     * instanced data. The only difference from the above is that the model 
     * variable is a vector.  
     * view is the position and orientation of the camera.  
     * projection is the perspective for which I use the perspective function
     * of GLM in the camera class. 
     * model is a vector of the various positions and orientations 
     * of the instances of the mesh being drawn.
     * lights are point lights defined by the PointLight structure.
     * spotLights are spotlights defined by the SpotLight structure.
     * viewPos is the camera position.
     * diffOnly when set to true will provide an image without specular highlights.
     * gamma controls brightness of the textured mesh.
     * total is the number of instances to be drawn.
     */
    void DrawInstanced(mat4 view, mat4 projection, vector<mat4>model, 
    vector<PointLight> lights, vector<SpotLight>spotLights, vec3 viewPos, 
    bool diffOnly, float gamma, unsigned int total);
    //! \brief For debugging.
    void dumpData();
    //! \brief Debug instance data.
    void debug(mat4 *modelData);
    //! \brief Debug uniform values for the shader.
    void uniforms();
    //! \brief Create the mesh data as an OpenGL buffer object.
    void setupMesh();
    //! \brief For debugging:  Print a vector of three floats.
    void printVec3(vec3 vecVal);
    //! \brief For debugging:  Print a 4x4 matrix of floats.
    void printMat4(mat4 matVal);
    /** \brief Blow up the object.
     * model is the position and orientation of the object.
     * view is the camera position and orientation.
     * projection is the Affine perspective matrix.
     * timeVal is the current frame number.
     */
    void explosion(mat4 model, mat4 view, mat4 projection, float timeVal);
    //! \brief A convenience function to pass messages.
    string getType();
    //! \brief A convenience function to post messages.
    void setType(string val);
    //! \brief For debugging:  Display an OpenGL error message.
    string getError();
    /* Variables */
    //! The index array.
    GLuint *indices;
    //! The associated textures as a vector -- contains texture buffers.
    vector<Texture>textures;
    //! Untextured color.
    vec3 colordiff = vec3(1.0f, 1.0f, 1.0f);
    //! The instance location and orientation array.
    mat4 *instanceArray;
    //! The shaders.
    Shader *shader, *explodeShader, *cloudShader;
    /*  Render data  */
    //! The buffer object handles.
    GLuint VAO, VBO[2], EBO;
    //! The upper bounds of the various buffers.
    int vertSize, indexSize, texSize, total;
    bool debug1 = false, start = true;
    //! Message data.
    string type;
    //! The object's opaqueness.
    float opacity = 1.0f;
    //! The instancing flag.
    bool instanced = false;
    //! The maximum instancing quantity and the uniform index.
    int quantity = 1, dataIndex = 0;
    //! The Vertex array.
    Vertex *vertices;
    //! The flat indexed Vertex array.
    Vertex *verticesIndexed;
    //! Sampler values.
    unsigned intdummyTex = 500;
    /** These booleans indicate no diffuse, specular or binormal textures
     * have been assigned.  When the program assigns a valid buffer 
     * value for that texture they are triggered to false.  Later they
     * are used to assign dummy values to buffers with no texture and
     * indicate that no texture of a given type is present.
     */
    bool difftrigger = true;
    bool spectrigger = true;
    bool heighttrigger = true;
    //! These booleans are switched to true if there is a diffuse, specular
    //! or binormal texture found.
    bool isDiff, isDiffTwo, isSpec, isBinorm, diffOnly;
    //! These integers contain the buffer values for the diffuse, specular
    //! and binormal textures.  If no texture is assigned a dummy value is given.
    int diffOne, diffTwo, specOne, binormOne, dummyTex = 500;
    /** These integers tell what the index of the texture is in the texture vector 
     * for each type of texture.
     * numDiff is the number of diffuse textures.
     */
    int diffOneX = 0, diffTwoX = 0, specOneX = 0, binormOneX = 0, numDiff = 0;  
    
    //! The explosion chunk data.
    //! The maximum possible number of chunks.
    int maxVal = 0;
    //! The chunk size vector.
    vector<int>chunks;
    //! The number of vertices.
    vec3 vert;
    //! The current vertex index for finding the normal to use as a direction vector.
    int currVal = 0;
    //! The size of the current chunk.
    int chunkSize = 0;
    //! The total number of chunks.
    int chunkCount = 0;
    //! A counter.
    int count = 0;
    //! The chunk's location and orientation.
    mat4 chunkLoc;
    //! The array of chunk locations and orientation.
    mat4 chunkArray[];
};


#endif // MESH_H
