/**********************************************************
 *   Model:  A class To extract 3D asset data from a blender
 *   file using the Assimp library, and pass it along to the 
 *   mesh classes.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   and originally found on www.learnopengl.com.
 *   March 2020 San Diego, California USA
 * ********************************************************/

#ifndef MODEL_H
#define MODEL_H

#include "commonheader.h"
#include "meshvert.h"
#include "meshtex.h"
#include "createimage.h"
#include "info.h"
#include "shader.h"

//! Forward declarations so it can be used as a library.
struct PointLight;
struct Texture;
struct SpotLight;
struct ModelInfo;
struct MeshInfo;
class Mesh;
/** \class Model A class to extract 3D asset data from a 
 * resource file and pass it along to the mesh files for 
 * display.
 */
class Model 
{
public:
    /** \brief Pass a vector containing file names of asset files 
     * and their associated positions and orientations.
     */
    Model(vector<ModelInfo> modelinfo, Shader *shader, int startIndex);
    /** \brief Pass a vector containing file names of asset files 
     * and their associated positions and orientations.
     * quantity : The number of instanced objects from each object.
     */
    Model(vector<ModelInfo> modelinfo, int quantity, Shader *shader, int startIndex);
    /** \brief Destructor, signals destruction of the class.
     * It deletes the textures and meshes.
     */
    ~Model();
    /* Functions */
    /** \brief Draw the assets that were obtained.  Pass along
     * the positions of the point lights and spotlights as well as 
     * the position and orientation of each object being displayed.
     * shader : An instance of the shader class. view is the position and orientation of
     * the camera.  
     * projection : The perspective for which I use the perspective function
     * of GLM in the camera class. 
     * model : The position and orientation of the mesh being drawn.
     * lights : The point lights defined by the PointLight structure.
     * spotLights : Spotlights defined by the SpotLight structure.
     * viewPos : The camera position.
     * startIndex : The current index of the textures being used.
     */
    void Draw(glm::mat4 view, glm::mat4 projection, vector<ModelInfo>model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos);   
    /** \brief Draw the assets that were obtained.  Pass along
     * the positions of the point lights and spotlights as well as 
     * the position and orientation of each object being displayed.
     * shader : An instance of the shader class. view is the position and orientation of
     * the camera.  
     * projection : The perspective for which I use the perspective function
     * of GLM in the camera class. 
     * model : The model definition data.
     * instanceData : The position and orientation of each instance.
     * lights : The point lights defined by the PointLight structure.
     * spotLights : Spotlights defined by the SpotLight structure.
     * viewPos : The camera position.
     * startIndex : The current index of the textures being used.
     */
    void DrawInstanced(mat4 view, mat4 projection, vector<ModelInfo>model, vector<mat4>instanceData, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos);
    //! \brief Accessor function to let the calling class know whether there are textures or not.
    bool hasTextures();
protected:
    /*  Functions   */
    //! \brief Open the asset for extraction. Uses the Assimp library to obtain the data.
    void loadModel(string path);
    //! \brief Process a node and all its subnodes, extracting meshes and textures.
    void processNode(aiNode* node, const aiScene* scene);
    //! \brief Extract the textures, vertices, indices, texture coordinates and others.
    MeshInfo processMesh(aiMesh* mesh, const aiScene* scene);
    //! \brief Compare distances
    static bool cmpdist(const ModelInfo &a, const ModelInfo &b);
    /** \brief Sort the objects by their distance from the
     *  camera.  Furthest first, closest last.
     */
    void sortDists(vec3 viewPos);
    //! \brief Compares ID values for sorting.
    static bool cmpid(const ModelInfo &a, const ModelInfo &b);
    /** \breif Sort the objects by idval.  We use this to reassign
     *  the meshes to each object when the draw function is called.
     */
    void sortIDs();
    /* Variables */
    //! \brief Get the texture from the assimp material file.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    /** \brief Get the image data from the file containing the texture, it
     *  uses Free Image Plus and the CreateImage class and creates an OpenGL bufferobject.
     */
    GLint TextureFromFile(string filename);
    //! \breif Print the instance data info.
    void debug(vector<mat4>instanceData);
    //! \breif Prints a three float vector. For debugging
    void printVec3(vec3 vecVal);
    //! \brief Print a 4x4 matrix.
    void printMat4(mat4 matVal);
    //! The model description vector. For information on the ModelInfo structure see the "info.h" file.
    vector<ModelInfo>modelinfo;
    //! A vector of Mesh classes, can be both MeshTex and MeshVert because
    //! they are both implementations of Mesh.
    vector<MeshInfo> meshes;
    //! The textures vector, one for each textured mesh. Texture is defined in "commonheader.h."
    vector<Texture>textures;
    //! The current mesh being worked on.
    aiMesh* mesh;
    //! The texture count start index.
    int startIndex;
    //! The index array.
    GLuint *indices = nullptr;
    //! The textured mesh vertex array.
    Vertex *vertices = nullptr;
    //! The untextured mesh vertex array.
    Vertex1 *vertices1 = nullptr;
    //! The image management class.
    CreateImage * imageMkr;
    //! The Assimp library importer.
    Assimp::Importer *import;
    //! The size of the Vertex, Index and Texture arrays respectively.
    int vertSize, indexSize;
    //! Texture flag.
    bool hasTex;
    //! Variables to hold the directory and filename for image access.
    string directory, filename;
    //! Book keeping variables.
    GLuint texcount = 0, vertcount = 0, count1 = 0, limit = 0;
    //! The alpha value for the shader.
    float opacity = 1.0f;
    //! The number of instances of each object.
    int quantity = -1;
    //! The number of instances per object.
    int span = 0;
    //! The instance shader.
    Shader *shader;
    //! The instance array.
    vector<mat4>modelData;
    //! Copious debug info to be had a the price of a single boolean value.
    bool debug1 = false;
    
};

#endif // MODEL_H
