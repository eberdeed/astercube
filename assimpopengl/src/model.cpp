/**********************************************************
 *   Model:  A class To extract 3D asset data using the Assimp
 *   library, and pass it along to the mesh classes.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   and originally found on www.learnopengl.com.
 *   April 2020 San Diego, California USA
 * ********************************************************/
#include "../include/model.h"
//! Load each asset one-by-one.
Model::Model(vector<ModelInfo> modelinfo, Shader *shader, int startIndex)
{
    cout << "\n\n\tCreating Model.\n\n";
    quantity = -1;
    this->startIndex = startIndex;
    imageMkr = new CreateImage();
    cout << "\n\n\tCreated Image Manager.\n\n";
    import = new Assimp::Importer();
    cout << "\n\n\tCreated Assimp Importer.\n\n";
    for (unsigned int x = 0; x < modelinfo.size(); x++)
    {
        texcount = vertcount = 0;
        cout << "\n\n\tLoading Model:  " << modelinfo[x].path << " Model Index:  " << x << ".\n\n";
        if (!exists(modelinfo[x].path))
        {
            cout << "\n\n\tError no model at " << modelinfo[x].path << ".\n\n";
            exit(-1);
        }
        
        loadModel(modelinfo[x].path);
        modelinfo[x].meshes = meshes;
        meshes.clear();
        textures.clear();
        limit = meshes.size();
        if (debug1)
        {
            cout << "\n\n\tProcessed " << texcount << " textured"
            << " meshes and " << vertcount << " untextured "
            << "meshes, for a total of " << limit << " meshes.\n\n";
        }
    }
    this->modelinfo = modelinfo;
}

Model::Model(vector<ModelInfo> modelinfo, int quantity, Shader *shader, int startIndex)
{
    cout << "\n\n\tCreating Model.\n\n";
    this->quantity = quantity;
    this->shader = shader;
    this->startIndex = startIndex;
    imageMkr = new CreateImage();
    cout << "\n\n\tCreated Image Manager.\n\n";
    import = new Assimp::Importer();
    cout << "\n\n\tCreated Assimp.\n\n";
    for (unsigned int x = 0; x < modelinfo.size(); x++)
    {
        texcount = vertcount = 0;
        cout << "\n\n\tLoading Model:  " << modelinfo[x].path << " Model Index:  " << x << ".\n\n";
        loadModel(modelinfo[x].path);
        modelinfo[x].meshes = meshes;
        limit = meshes.size();
        meshes.clear();
        textures.clear();
        if (debug1)
        {
            cout << "\n\n\tProcessed " << texcount << " textured"
            << " meshes and " << vertcount << " untextured "
            << "meshes, for a total of " << limit << " meshes.\n\n";
        }
    }
    this->modelinfo = modelinfo;
}

Model::~Model()
{
    cout << "\n\n\tDestroying " 
    << modelinfo.size() << " Models.\n\n";
    for (int x = 0; x < modelinfo.size(); x++)
    {
        for (int y = 0; y < modelinfo[x].meshes.size(); y++)
        {
            for (int z = 0; z < modelinfo[x].meshes[y].textures.size(); z++)
            {
               glDeleteTextures(1, &modelinfo[x].meshes[y].textures[z].id);
               if (debug1)
               {
                   cout << "\n\tDestroying texture " << z << " in mesh " << y 
                   << " in object " << x;
               }
                
            }
        }
    }
    modelinfo.clear();
    delete imageMkr;
    cout << "\n\n\tModel deleted.\n\n";
}
//! Draw each asset as a series of meshes.
void Model::Draw(mat4 view, mat4 projection, vector<ModelInfo>model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos)
{
    MeshInfo meshItem;
    string type;
    shader->Use();
    sortIDs();
    for (int x = 0; x < modelinfo.size(); x++)
    {
        modelinfo[x].model = model[x].model;
        modelinfo[x].location = model[x].location;
        modelinfo[x].gamma = model[x].gamma;
    }
    sortDists(viewPos);
    if (debug1)
    {
        cout << "\n\tView Position:  ";
        printVec3(viewPos);
    }
    for (int y = 0; y < modelinfo.size(); y++)
    {
        meshes.clear();
        meshes = modelinfo[y].meshes;
        if(debug1)
        {
            cout << "\n\tLocation:  ";
            printVec3(modelinfo[y].location);
        }
        int limit = meshes.size();
        for (int x = 0; x < limit; x++)
        {
            meshItem = meshes[x];
            type = meshItem.mesh->getType();
            if(debug1)
            {
                cout << "\n\tDrawing mesh " << x << " from model " << modelinfo[y].path 
                << " of type " << type << " with gamma " << modelinfo[y].gamma;
            }
            meshItem.mesh->Draw(view, projection, modelinfo[y].model, lights, spotLights, viewPos, modelinfo[y].diffOnly, modelinfo[y].gamma);
            startIndex += modelinfo[y].meshes[x].textures.size();
            if (debug1)
            {
                cout << "\n\t:  " << startIndex;
            }
        }
    }
}  

void Model::debug(vector<mat4>instanceData)
{
    for (int y = 0; y < quantity; y++)
    {
        cout << "\nAsteroid Number " << y + 1 << "\n";
        printMat4(instanceData[y]);
        
    }
}
            
//! Draw each asset as a series of meshes.
void Model::DrawInstanced(mat4 view, mat4 projection, vector<ModelInfo>model, vector<mat4>instanceData, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos)
{
    if (debug1)
    {
        debug(instanceData);
    }
    shader->Use();
    MeshInfo meshItem;
    mat4 tmpMat;
    string type;
    int start = 0;
    int end = quantity;
    int count;
    if (debug1)
    {
        cout << "\n\tView Position:  ";
        printVec3(viewPos);
    }
    for (int y = 0; y < modelinfo.size(); y++)
    {
        modelData.clear();
        count = 0;
        for (int x = start; x < end; x++)
        {
            modelData.push_back(instanceData[x]);
        }
        start += quantity;
        end += quantity;
        meshes = modelinfo[y].meshes;
        int limit = meshes.size();
        for (int x = 0; x < limit; x++)
        {
            meshItem = meshes[x];
            type = meshItem.mesh->getType();
            if(debug1)
            {
                cout << "\n\tDrawing mesh " << x << " from model " << modelinfo[y].path 
                << " of type " << type << " with gamma " << modelinfo[y].gamma;
            }
            if (debug1)
            {
                for (int x = 0; x < quantity; x++)
                {
                    cout << "\n\tData for asteroid " << x << " in the Model class.";
                    printMat4(modelData[x]);
                }
            }
            meshItem.mesh->DrawInstanced(view, projection, modelData, lights, spotLights, viewPos, modelinfo[y].diffOnly, modelinfo[y].gamma);
            startIndex += modelinfo[y].meshes[x].textures.size();
            if (debug1)
            {
                cout << "\n\t:  " << startIndex;
            }
        }
    }
}  

//! Less than operator for stable_sort.
bool Model::cmpdist(const ModelInfo &a, const ModelInfo &b)
{   
    return a.dist > b.dist;
}


void Model::sortDists(vec3 viewPos)
{
    for (int x = 0; x < modelinfo.size(); x++)
    {
        vec3 location = modelinfo[x].location;
        modelinfo[x].dist = distance(location, viewPos);
        //!cout << "\n\n\tDistance: " << x << " : " << modelinfo[x].dist;
    }
    //! Sort uses the algorithm library.
    sort(modelinfo.begin(), modelinfo.end(), cmpdist);
}
//! Less than operator for stable_sort.
bool Model::cmpid(const ModelInfo &a, const ModelInfo &b)
{   
    return a.idval < b.idval;
}


void Model::sortIDs()
{
    //! Sort uses the algorithm library.
    sort(modelinfo.begin(), modelinfo.end(), cmpid);
}


//! Read an asset file using the Assimp library.
void Model::loadModel(string path)
{
    cout << "\n\n\tLoading design:  " << path << "\n\n";
    hasTex = false;
    const aiScene* scene = import->ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals 
    | aiProcess_GenUVCoords);
    cout << "\n\n\tLoaded scene.\n\n"; 
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        cout << "Error upon Assimp library import: " << import->GetErrorString() << endl;
        exit(1);
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
    if (debug1)
    {
        for (int y = 0; y < meshes.size(); y++)
        {
            cout <<  "\n\n\tTextures for mesh " << y << "\n";
            for (int x = 0; x < textures.size(); x++)
            {
                cout << "\n\tTexture " << x << " of type " << textures[x].type 
                << " at location " << textures[x].path;
            }
        }
    }
}

//! Process a node and all subnodes.
void Model::processNode(aiNode* node, const aiScene* scene)
{
    cout << "\n\n\tIn processNode.\n\n";
    // Process all the node's meshes (if any)
    if (node->mNumMeshes > 0)
    {
        cout << "\n\n\tProcessing " << node->mNumMeshes << " nodes.\n\n";
    }
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene)); 
    }
    // Then do the same for each of its children
    cout << "\n\n\tProcessing " << node->mNumChildren << " child nodes.\n\n";
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
} 

//! Process a single mesh.
MeshInfo Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    MeshInfo item;
    vec3 colordiff;
    aiVector3D *texptr = nullptr;
    aiVector3D texval;
    indexSize = vertSize = 0;
    // Data to fill
    Mesh *meshPtr;
    int vertSize;
    textures.clear();
    if (mesh->HasTextureCoords(0))
    {
        hasTex = true;
        cout << "\n\n\tHas texture coordinates.\n\n";
    }
    else
    {
        hasTex = false;
        cout << "\n\n\tDoes not have texture coordinates.\n\n";
    }
    bool colors = mesh->HasVertexColors(0);
    if (colors)
    {
        cout << "\n\n\tThis mesh has vertex colors.\n\n";
    }
    else
    {
        cout << "\n\n\tThis mesh does not have vertex colors.\n\n";
    }
    try
    {
        vertSize = mesh->mNumVertices;
        if (vertSize >= 3)
        {
            //! Textured mesh.
            if (hasTex)
            {
                vertices = new Vertex[vertSize];
                // Walk through each of the mesh's vertices
                for(GLuint i = 0; i < mesh->mNumVertices; i++)
                {
                    // Positions
                    vertices[i].Position[0] = mesh->mVertices[i].x;
                    vertices[i].Position[1] = mesh->mVertices[i].y;
                    vertices[i].Position[2] = mesh->mVertices[i].z;
                    vertices[i].Normal[0] = mesh->mNormals[i].x;
                    vertices[i].Normal[1] = mesh->mNormals[i].y;
                    vertices[i].Normal[2] = mesh->mNormals[i].z;
                    texptr = nullptr;
                    texptr = &mesh->mTextureCoords[0][i];
                    if (!texptr)
                    {
                        cout << "\n\n\tNo texture coordinates.\n\n";
                        exit(1);
                    }
                    else
                    {
                        vertices[i].TexCoords[0] = texptr->x;
                        vertices[i].TexCoords[1] = texptr->y;
                    }
                 }
                if (mesh->HasFaces())
                {
                    vector<GLuint>indices1;
                    for(GLuint i = 0; i < mesh->mNumFaces; i++)
                    {
                        aiFace face = mesh->mFaces[i];
                        // Retrieve all indices of the face and store them in the indices vector
                        for(GLuint j = 0; j < face.mNumIndices; j++)
                        {
                            indices1.push_back(face.mIndices[j]);
                        }
                    }
                    indexSize = indices1.size();
                    indices = new GLuint[indexSize];
                    for (int x = 0; x < indexSize; x++)
                    {
                        indices[x]  = indices1[x];
                   }
                }
                if (mesh->mMaterialIndex >= 0)
                {
                    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                    if (debug1)
                    {
                        aiMaterialProperty **proplist = material->mProperties;
                        cout << "\n\n\tProperty list for material "  << mesh->mMaterialIndex << "\n";
                        for (int x = 0; x < material->mNumProperties; x++)
                        {
                            cout << "\n\t" << proplist[x]->mData << " : " << proplist[x]->mKey.C_Str();
                        }
                    }
                    material->Get(AI_MATKEY_OPACITY, opacity);
                    /** Here we check for all the texture types even though we are only using three:
                     *  diffuse - color, specular - highlights, height - bumps.
                     */
                    vector<Texture> noneMaps = loadMaterialTextures(material, aiTextureType_NONE, "none");
                    textures.insert(textures.end(), noneMaps.begin(), noneMaps.end());
                    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
                    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
                    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
                    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
                    vector<Texture> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "ambient");
                    textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
                    vector<Texture> emissiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "emissive");
                    textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
                    vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "height");
                    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
                    vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "normal");
                    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
                    vector<Texture> shininessMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "shininess");
                    textures.insert(textures.end(), shininessMaps.begin(), shininessMaps.end());
                    vector<Texture> opacityMaps = loadMaterialTextures(material, aiTextureType_OPACITY, "opacity");
                    textures.insert(textures.end(), opacityMaps.begin(), opacityMaps.end());
                    vector<Texture> displacementMaps = loadMaterialTextures(material, aiTextureType_DISPLACEMENT, "displacement");
                    textures.insert(textures.end(), displacementMaps.begin(), displacementMaps.end());
                    vector<Texture> lightMaps = loadMaterialTextures(material, aiTextureType_LIGHTMAP, "lightmap");
                    textures.insert(textures.end(), lightMaps.begin(), lightMaps.end());
                    vector<Texture> unknownMaps = loadMaterialTextures(material, aiTextureType_UNKNOWN, "unknown");
                    textures.insert(textures.end(), unknownMaps.begin(), unknownMaps.end());
                }
                MeshTex *meshTexPtr = new MeshTex();
                if (quantity > 0)
                {
                    startIndex = meshTexPtr->setData(vertices, indices, textures, vertSize, indexSize, true, quantity, shader, startIndex);
                }
                else
                {
                    startIndex = meshTexPtr->setData(vertices, indices, textures, vertSize, indexSize, false, -1, shader, startIndex);
                }
                opacity = glm::clamp(opacity, 0.0f, 1.0f);
                meshTexPtr->opacity = opacity;
                meshTexPtr->setType("Textured");
                meshPtr = meshTexPtr;
                texcount++;
            }
            else
            {
                //! Untextured mesh.
                vertices1 = new Vertex1[vertSize];
                
                // Walk through each of the mesh's vertices.
                for(GLuint i = 0; i < mesh->mNumVertices; i++)
                {
                    // Positions
                    vertices1[i].Position[0] = mesh->mVertices[i].x;
                    vertices1[i].Position[1] = mesh->mVertices[i].y;
                    vertices1[i].Position[2] = mesh->mVertices[i].z;
                    vertices1[i].Normal[0] = mesh->mNormals[i].x;
                    vertices1[i].Normal[1] = mesh->mNormals[i].y;
                    vertices1[i].Normal[2] = mesh->mNormals[i].z;
                }
                if (mesh->HasFaces())
                {
                    vector<GLuint>indices1;
                    for(GLuint i = 0; i < mesh->mNumFaces; i++)
                    {
                        aiFace face = mesh->mFaces[i];
                        // Retrieve all indices of the face and store them in the indices vector
                        for(GLuint j = 0; j < face.mNumIndices; j++)
                        {
                            indices1.push_back(face.mIndices[j]);
                        }
                    }
                    indexSize = indices1.size();
                    indices = new GLuint[indexSize];
                    for (int x = 0; x < indexSize; x++)
                    {
                        indices[x]  = indices1[x];
                    }
                }
                if (mesh->mMaterialIndex >= 0)
                {
                    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                    if (debug1)
                    {
                        aiMaterialProperty **proplist = material->mProperties;
                        cout << "\n\n\tProperty list:  ";
                        for (int x = 0; x < material->mNumProperties; x++)
                        {
                            cout << "\n\t" << proplist[x]->mData << " : " << proplist[x]->mKey.C_Str();
                        }
                    }
                    aiColor4D color (0.0f, 0.0f, 0.0f, 0.0f);
                    material->Get(AI_MATKEY_COLOR_DIFFUSE,color);
                    cout << "\n\n\tMaterial color:  "
                    << color[0] << ", " << color[1] << ", "
                    << color[3] << ", " << color[3];
                    material->Get(AI_MATKEY_OPACITY, opacity);
                    for (int x = 0; x < 3; x++)\
                    {
                        colordiff[x] = color[x];
                    }
                }
                MeshVert *meshVertPtr = new MeshVert();
                if (quantity > 0)
                {
                    startIndex = meshVertPtr->setData(vertices1, indices, colordiff, vertSize, indexSize, true, quantity, shader, startIndex);
                }
                else
                {
                    startIndex = meshVertPtr->setData(vertices1, indices, colordiff, vertSize, indexSize, false, -1, shader, startIndex);
                }
                if (opacity  > 1.0f)
                {
                    opacity = 0.7f;
                }
                meshVertPtr->opacity = opacity;
                vertcount++;
                meshVertPtr->setType("Untextured");
                meshPtr = meshVertPtr;
            }
        }
        else
        {
            cout << "\n\nFatal Error:  Mesh with no vertices.\n";
            exit(1);
        }
        item.mesh = meshPtr;
        item.textures = textures;
    }
    catch(exception exc)
    {
        cout << "\n\n\tFatal Error Loading Mesh:  " << exc.what() << "\n\n";
        exit(1);
    }
    // Return a mesh object created from the extracted mesh data
    if (debug1)
    {
        cout << "\n\n\tProcessed one " << meshPtr->type << " mesh.\n\n";
    }
    return item;
}

//! Check for existing texture and load as needed.
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        filename = string(str.C_Str());
        filename = directory + "/" + filename;
        if (debug1)
        {
            cout << "\n\n\tTexture directory:  " << directory << " filename:  " << filename 
            << "  type:  " << typeName << "  count:  " << (i + 1) << "\n\n";
        }
        GLboolean skip = false;
        Texture texture;
        texture.id = TextureFromFile(filename);
        if (texture.id > 0)
        {
            texture.type = typeName;
            texture.path = filename;
            textures.push_back(texture);
        }
    }
    return textures;
}  

//! Use the CreateImage class to turn an image into a texture.
GLint Model::TextureFromFile(string filename)
{
    //Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    GLsizei width, height;
    if (debug1)
    {
        cout << "\n\n\tProcessing:  " << filename << "\n\n";
    }
    if(imageMkr->setImage(filename))
    {
        textureID = imageMkr->textureObject();
        if (debug1)
        {
            cout << "\n\n\tReturning texture buffer:  " << textureID << "\n\n";
        }
        return textureID;
    }
    else
    {
        return 0;
    }
}

//! Let SceneMkr the calling class know if there are images as textures.
bool Model::hasTextures()
{
    return hasTex;
}

void Model::printVec3(vec3 vecVal)
{
    cout << "  3 Float Vector:  ";
    for  (int x = 0; x < 3; x++)
    {
        cout << vecVal[x] << ", ";
    }
}

void Model::printMat4(mat4 matVal)
{
    cout << "  4x4 Matrix\n\t";
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            cout << matVal[y][x] << ", ";
        }
        cout << "\n\t";
    }
}

