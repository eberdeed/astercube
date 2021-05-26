/**********************************************************
 *   Model:  A class To extract 3D asset data using the Assimp
 *   library, and pass it along to the mesh classes.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   and originally found on www.learnopengl.com.
 *   April 2020 San Diego, California USA
 * ********************************************************/
#include "../include/model.h"
//! Load each asset one-by-one.
Model::Model(vector<ModelInfo> modelinfo, Shader *shader, Shader *explodeShader, 
Shader *cloudShader, int startIndex)
{
    cout << "\n\n\tCreating Model.\n\n";
    quantity = -1;
    this->startIndex = startIndex;
    this->shader = shader;
    this->explodeShader = explodeShader;
    this->cloudShader = cloudShader;
    cloud = new SmokeCloud(cloudShader);
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
        limit = meshes.size();
        modelinfo[x].meshes = meshes;
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
    delete import;
}

Model::Model(vector<ModelInfo> modelinfo, Shader *shader, Shader *explodeShader, 
Shader *cloudShader, int startIndex, int quantity)
{
    cout << "\n\n\tCreating Model.\n\n";
    this->quantity = quantity;
    this->shader = shader;
    this->explodeShader = explodeShader;
    this->cloudShader = cloudShader;
    this->startIndex = startIndex;
    cloud = new SmokeCloud(cloudShader);
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
    delete import;
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
void Model::Draw(int index, mat4 view, mat4 projection, vector<ModelInfo>model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos)
{
    MeshInfo meshItem;
    string type;
    sortIDs();
    modelinfo[index].model = model[index].model;
    modelinfo[index].location = model[index].location;
    modelinfo[index].gamma = model[index].gamma;
    sortDists(viewPos);
    if (debug1)
    {
        cout << "\n\tView Position:  ";
        printVec3(viewPos);
    }
    meshes.clear();
    meshes = modelinfo[index].meshes;
    if(debug1)
    {
        cout << "\n\tLocation:  ";
        printVec3(modelinfo[index].location);
        printMat4(modelinfo[index].model);
    }
    int limit = meshes.size();
    for (int x = 0; x < limit; x++)
    {
        meshItem = meshes[x];
        type = meshItem.mesh->getType();
        if(debug1)
        {
            cout << "\n\tDrawing mesh " << x << " from model " << modelinfo[index].path 
            << " of type " << type << " with gamma " << modelinfo[index].gamma;
        }
        meshItem.mesh->Draw(view, projection, modelinfo[index].model, lights, spotLights, viewPos, modelinfo[index].diffOnly, modelinfo[index].gamma);
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
void Model::DrawInstanced(mat4 view, mat4 projection, vector<ModelInfo>model, vector<mat4>instanceData, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, unsigned int totals[])
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
            if (totals[y] > 0)
            {
                if (debug1)
                {
                    cout << "\n\tDrawing object " << y 
                    << " with start and end " << start 
                    << " - " << end << ".\n\n";
                }
                meshItem.mesh->DrawInstanced(view, projection, modelData, lights, spotLights, viewPos, modelinfo[y].diffOnly, modelinfo[y].gamma, totals[y]);
            }
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
                Mesh *meshTexPtr = new Mesh();
                if (quantity > 0)
                {
                    startIndex = meshTexPtr->setData(vertices, indices, textures, colordiff, vertSize, indexSize, true, quantity, shader, explodeShader, startIndex);
                }
                else
                {
                    startIndex = meshTexPtr->setData(vertices, indices, textures, colordiff, vertSize, indexSize, false, -1, shader, explodeShader, startIndex);
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
                vertices = new Vertex[vertSize];
                
                // Walk through each of the mesh's vertices.
                for(GLuint i = 0; i < mesh->mNumVertices; i++)
                {
                    // Positions
                    vertices[i].Position[0] = mesh->mVertices[i].x;
                    vertices[i].Position[1] = mesh->mVertices[i].y;
                    vertices[i].Position[2] = mesh->mVertices[i].z;
                    vertices[i].Normal[0] = mesh->mNormals[i].x;
                    vertices[i].Normal[1] = mesh->mNormals[i].y;
                    vertices[i].Normal[2] = mesh->mNormals[i].z;
                    vertices[i].TexCoords[0] = 0.0f;
                    vertices[i].TexCoords[1] = 0.0f;
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
                Mesh *meshVertPtr = new Mesh();
                textures.clear();
                if (quantity > 0)
                {
                    startIndex = meshVertPtr->setData(vertices, indices, textures, colordiff, vertSize, indexSize, true, quantity, shader, explodeShader, startIndex);
                }
                else
                {
                    startIndex = meshVertPtr->setData(vertices, indices, textures, colordiff, vertSize, indexSize, false, -1, shader, explodeShader, startIndex);
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

float Model::DrawExplosion(int index, mat4 view, mat4 projection, vector<ModelInfo>modelobj)
{
    MeshInfo meshItem;
    string type;
    meshes.clear();
    meshes = modelinfo[index].meshes;
    if(debug1)
    {
        cout << "\n\tExplosion Location:  ";
        printVec3(modelobj[index].location);
    }
    try
    {
        if (timeVal == 0.0f)
        {
            /* Simply create a thread */
            thread = SDL_CreateThread((SDL_ThreadFunction)Model::sndMaker, "SoundThread", (void *)NULL);
            if (NULL == thread) 
            {
                cout << "SDL_CreateThread failed";
                exit(-1);
            } 
            else 
            {
                if (debug1)
                {
                    cout << "\n\n\tThread successfully created.\n\n";
                }
                SDL_DetachThread(thread);
            }
        }
    }
    catch (exception exc)
    {
        cout << "\n\n\tError creating sound thread:  " << exc.what() << ".\n\n";
        exit(-1);
    }
    int limit = meshes.size();
    //cout << "\n\tLimit " << limit;
    cloud->drawCloud(modelobj[index].model, view, timeVal);
    for (int x = 0; x < limit; x++)
    {
        meshItem = meshes[x];
        type = meshItem.mesh->getType();
        if(true)
        {
            cout << "\n\tDrawing mesh " << x << " from model " << modelinfo[index].path 
            << " of type " << type << " with gamma " << modelinfo[index].gamma;
            cout << "\n\tExplosion Matrix: \n";
            printMat4(modelobj[index].model);
            cout << "\n\tExplosion View Matrix:\n";
            printMat4(view);
        }
        meshItem.mesh->explosion(modelobj[index].model, view, projection, timeVal);
    }
    timeVal += 1.0f;
    return timeVal;
}
void Model::sndMaker()
{
    Uint32 wave_len; // length of our sample
    Uint8 *wave_buf; // buffer containing our audio file
    SDL_AudioSpec wave_spec, ret_spec; // The specs of our piece of music.
    SDL_AudioDeviceID dev; //The playback device.
    if (debug1)
    {
        cout << "\n\n\tStarting sound production.";
        cout << "\n\tLoading WAV file.\n\n";
    }
    try
    {
        
        if (SDL_LoadWAV(explosionSound, &wave_spec, &wave_buf, &wave_len) ==  NULL)
        {
            cout << "\n\n\tUnable to load WAV file:  " << SDL_GetError() << "\n\n";
        }
        else
        {
            if (debug1)
            {
                cout << "\n\n\tWAV file successfully loaded.\n\n";
            }
        }
        string name  = SDL_GetAudioDeviceName(0, 0);
        cout << "\n\n\tAudio device name:  " << name << ".\n\n";
        dev = SDL_OpenAudioDevice(name.c_str(), 0, &wave_spec, &ret_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
        if (dev > 0)
        {
            if (debug1)
            {
                if (debug1)
                {
                    cout << "\n\n\tAudio device successfully opened.\n\n";
                }
            }    
            int result = SDL_QueueAudio(dev, wave_buf, wave_len);
            if (result != 0)
            {
                cout <<  "\n\n\tUnable to queue audio data.\n\n";
            }
            else
            {
                if (debug1)
                {
                    cout << "\n\n\tAudio data successfully queued.\n\n";
                }
            }
            SDL_PauseAudioDevice(dev, 0); /* start audio playing. */
            if (debug1)
            {
                cout << "\n\n\tPlayed the audio.\n\n";
            }
            SDL_Delay(EXP_GAP); /* let the audio callback play some sound for EXP_GAP seconds. */
            if (debug1)
            {
                cout << "\n\n\tClosed audio device.\n\n";
            }
        }
    }
    catch (exception exc)
    {
        cout << "\n\n\tError opening SDL2 audio device:  " << exc.what() << ".\n\n";
    }
    SDL_ClearQueuedAudio(dev);
    SDL_CloseAudioDevice(dev);
    SDL_FreeWAV(wave_buf);
}

void Model::zeroTime()
{
    timeVal = 0.0f;
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

