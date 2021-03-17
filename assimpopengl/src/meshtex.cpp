/**********************************************************
 *   MeshTex:  A class that is derived from Mesh. This class
 *   is for textured meshes.  This class isolates tha
 *   display of data from a 3d asset file using assimp.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>,
 *   it was originally found on www.learnopengl.com.
 *   April 2020 San Diego, California USA
 * ********************************************************/
#include "../include/meshtex.h"

MeshTex::MeshTex()
{
    cout << "\n\n\tCreating MeshTex.\n\n";
    return;
}
    
MeshTex::~MeshTex()
{
    cout << "\n\n\tDestroying MeshTex.\n\n";
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO[0]);
    glDeleteBuffers(1, &EBO);
}
void MeshTex::debug(mat4 *modelData)
{
    for (int y = 0; y < quantity; y++)
    {
        cout << "\n\tAsteroid:  " << y << " in the MeshTex class.";
        printMat4(modelData[y]);
    }
        
}

// Pass along data from the model class to be drawn here.
int MeshTex::setData(Vertex *vertices, GLuint *indices, vector<Texture>textures, int vertSize, int indexSize, bool instanced, int quantity, Shader *shader, int startIndex)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->vertSize = vertSize;
    this->indexSize = indexSize;
    this->instanced = instanced;
    this->quantity = quantity;
    this->shader = shader;
    if ((instanced) && (quantity > 0))
    {
        instanceArray = new mat4[quantity];
    }
    numDiff = 0;
    //! Bind appropriate textures
    //! Here we allow for the three types of textures: Diffuse, specular and binormal or bumpmap.
    for( int x = 0; x < textures.size(); x++)
    {
        //! Texture present.
        if (debug1)
        {
            cout << "\n\t Texture Path:  " << textures[x].path << "  Texture Type:  " << textures[x].type 
            << "  Index:  " << startIndex + x << " gamma " << gamma;
        }
        if ((textures[x].type == "diffuse"))
        {
            if (!difftrigger)
            {
                diffTwo = startIndex + x;
                diffTwoX = x;
                isDiffTwo = true;
                numDiff = 2;
            }
            else
            {
                diffOne = startIndex + x;
                diffOneX = x;
                dummyTex++;
                diffTwo = dummyTex + startIndex;
                isDiffTwo = false;
                numDiff = 1;
            }
            isDiff = true;
            difftrigger = false;
        }
        else if (((textures[x].type == "shininess") || (textures[x].type == "specular")) && (spectrigger))
        {
            spectrigger = false;
            isSpec = true;
            specOne = startIndex + x;
            specOneX = x;
        }
        else if (((textures[x].type == "height") || (textures[x].type == "normal")) && (heighttrigger))
        {
            isBinorm = true;
            binormOne = startIndex + x;
            binormOneX = x;
            heighttrigger = false;
        }
        dummyTex++;
    }
    if (difftrigger)
    {
       isDiff = false;
       diffOne = dummyTex++ + startIndex;
       diffTwo = dummyTex++ + startIndex;
    }
    if (spectrigger)
    {
       isSpec = false;
       specOne = dummyTex++ + startIndex;
   }
    if (heighttrigger)
    {
       isBinorm = false;
       binormOne = dummyTex++ + startIndex;
    }
    if (instanced)
    {
        setupInstancedMesh();
    }
    else
    {
        setupMesh();
    }
    if (debug1)
    {
        dumpData();
    }
    return (startIndex + 4);
}

//! For debugging.
void MeshTex::dumpData()
{
    cout << "\n\n\tVertices, Normals and TexCoords: \n\n";
    for (int x = 0; x < vertSize; x++)
    {
        cout << "\n\tVertex: " << vertices[x].Position[0] << ", " << vertices[x].Position[1] << ", " << vertices[x].Position[2] <<
        " Normal: " << vertices[x].Normal[0] << ", " << vertices[x].Normal[1] << ", " << vertices[x].Normal[2] <<
        " TexCoord: " << vertices[x].TexCoords[0] << ", " << vertices[x].TexCoords[1];
    }
    if (instanced)
    {
        for (int x = 0; x < indexSize; x++)
        {
            cout << "\n\tIndexed Vertex: " << verticesIndexed[x].Position[0] << ", " << verticesIndexed[x].Position[1] << ", " << verticesIndexed[x].Position[2] <<
            " Normal: " << verticesIndexed[x].Normal[0] << ", " << verticesIndexed[x].Normal[1] << ", " << verticesIndexed[x].Normal[2] <<
            " TexCoord: " << verticesIndexed[x].TexCoords[0] << ", " << verticesIndexed[x].TexCoords[1];
        }
    }
    cout << "\n\n\n\tIndices: \n\n";
    for (int x = 0; x < indexSize; x++)
    {
        cout << " " << indices[x];
        if (((x + 1) % 25) == 0)
        {
            cout << "\n\n";
        }
    }
    if (texSize > 0)
    {
        for (int x = 0; x < textures.size(); x++)
        {
            cout << "\n\tTexture ID " << textures[x].id 
            << " for texture type " << textures[x].type;
        }
    }
}

//! Allocate the vertex array and index buffer.
void MeshTex::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO[0]);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW); 
   
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(GLuint), indices, GL_STATIC_DRAW);
    
    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);   
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}  

void MeshTex::setupInstancedMesh()
{
    /*
    struct Vertex {
        //! Vertex Postion cooridinates.
        float Position[3];
        //! Normal to the surface at the vertex point.
        float Normal[3];
        //! Texture coordinates.
        float TexCoords[2];
    };
    */
    verticesIndexed = new Vertex[indexSize];
    for (int x = 0; x < indexSize; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            verticesIndexed[x].Position[y] = vertices[indices[x]].Position[y];
            verticesIndexed[x].Normal[y] = vertices[indices[x]].Normal[y];
            if (y < 2)
            {
                verticesIndexed[x].TexCoords[y] = vertices[indices[x]].TexCoords[y];
            }
        }
    }
    if (debug1)
    {
        dumpData();
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, indexSize * sizeof(Vertex), verticesIndexed, GL_STATIC_DRAW); 
   
    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);   
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); 
    //! Uniform buffer to feed the uniform.
 
}
//! Draw the object.
void MeshTex::Draw(mat4 view, mat4 projection, mat4 model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, bool diffOnly, float gamma) 
{
    glBindVertexArray(VAO);
    bool difftrigger = true;
    bool spectrigger = true;
    bool heighttrigger = true;
    opacity = 1.0f;
    
    shader->setFloat("gamma", gamma);
    shader->setBool("diffOnly", diffOnly);
    //! Bind appropriate textures
    //! Here we allow for the three types of textures: Diffuse, specular and binormal or bumpmap.
    if (isDiff)
    {
        glActiveTexture(GL_TEXTURE0  + diffOne); // Active proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, textures[diffOneX].id);
    }
    shader->setBool("isDiffuse", isDiff);
    shader->setInt("diffuseOne", diffOne);
    if (isDiffTwo)
    {
        glActiveTexture(GL_TEXTURE0  + diffTwo); // Active proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, textures[diffTwoX].id);
    }
    shader->setInt("diffuseTwo", diffTwo);
    if (isSpec)
    {
        glActiveTexture(GL_TEXTURE0  + specOne); // Active proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, textures[specOneX].id);
    }
    shader->setBool("isSpecular", isSpec);
    shader->setInt("specularOne", specOne);
    if (isBinorm)
    {
        glActiveTexture(GL_TEXTURE0  + binormOne); // Active proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, textures[binormOneX].id);
    }
    shader->setInt("numDiffuse", numDiff);
    shader->setBool("isBinormal", isBinorm);
    shader->setInt("binormalOne", binormOne);
    shader->setFloat("shininess", 10.0f);
    shader->setVec3("viewPos", viewPos);
    shader->setFloat("opacity", opacity);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", model);
    shader->setVec3("colordiff", vec3(1.0f, 1.0f, 1.0f));
    for (int x = 0; x < lights.size(); x++)
    {   
        stringstream ss;
        ss << x;
        shader->setVec3("pointLights[" + ss.str() + "].position", lights[x].position);
        shader->setVec3("pointLights[" + ss.str() + "].ambient", lights[x].ambient);
        shader->setVec3("pointLights[" + ss.str() + "].diffuse", lights[x].diffuse);
        shader->setVec3("pointLights[" + ss.str() + "].specular", lights[x].specular);
        shader->setFloat("pointLights[" + ss.str() + "].constant", lights[x].constant);
        shader->setFloat("pointLights[" + ss.str() + "].linear", lights[x].linear);
        shader->setFloat("pointLights[" + ss.str() + "].quadratic", lights[x].quadratic);
    }
    for (int x = 0; x < spotLights.size(); x++)
    {   
        stringstream ss;
        ss << x;
        shader->setVec3("spotLights[" + ss.str() + "].position", spotLights[x].position);
        shader->setVec3("spotLights[" + ss.str() + "].direction", spotLights[x].direction);
        shader->setFloat("spotLights[" + ss.str() + "].cutOff", spotLights[x].cutOff);
        shader->setFloat("spotLights[" + ss.str() + "].outerCutOff", spotLights[x].outerCutOff);
        shader->setVec3("spotLights[" + ss.str() + "].ambient", spotLights[x].ambient);
        shader->setVec3("spotLights[" + ss.str() + "].diffuse", spotLights[x].diffuse);
        shader->setVec3("spotLights[" + ss.str() + "].specular", spotLights[x].specular);
        shader->setFloat("spotLights[" + ss.str() + "].constant", spotLights[x].constant);
        shader->setFloat("spotLights[" + ss.str() + "].linear", spotLights[x].linear);
        shader->setFloat("spotLights[" + ss.str() + "].quadratic", spotLights[x].quadratic);
    }
    if (debug1)
    {
        cout << "\n\tModel in MeshTex:  ";
        printMat4(model);
        cout << "\n\tCamera Position in MeshTex:  ";
        printVec3(viewPos);
    }
    // Draw mesh
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

//! Draw the object.
void MeshTex::DrawInstanced(mat4 view, mat4 projection, vector<mat4>model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, bool diffOnly, float gamma) 
{
    for (int x = 0; x < quantity; x++)
    {
        instanceArray[x] = model[x];
    }
    if (debug1)
    {
        debug(instanceArray);
        cout << "\n\n\tIs instanced:  " << instanced << "\n\n";
    }
    glBindVertexArray(VAO);
    bool difftrigger = true;
    bool spectrigger = true;
    bool heighttrigger = true;
    opacity = 1.0f;
    
    shader->setFloat("gamma", gamma);
    shader->setBool("diffOnly", diffOnly);
    //! Bind appropriate textures
    //! Here we allow for the three types of textures: Diffuse, specular and binormal or bumpmap.
     cout  << "\n\n\tSampler IDs 1: diffOne " << diffOne 
    << " diffTwo " << diffTwo << " specOne " << specOne
    << " binormOne " << binormOne;
    shader->Use();
    shader->setFloat("gamma", gamma);
    shader->setBool("diffOnly", diffOnly);
    if (debug1)
    {
        debug(instanceArray);
        cout << "\n\n\tIs instanced:  " << instanced << "\n\n";
        cout << "\n\n\tView: ";
        printMat4(view);
        cout << "\n\n\tProjection: ";
        printMat4(projection);
        cout << "\n\n\tSize of lights: " << lights.size();
        cout << "\n\n\tSize of spotLights: " << spotLights.size();
        cout << "\n\n\tBuffer ID for positions: " << VBO[1] << ".\n";
    }
    glBindVertexArray(VAO);
    shader->setInt("numDiffuse", numDiff);
    if (isDiff)
    {
        glActiveTexture(GL_TEXTURE0  + diffOne); // Active proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, textures[diffOneX].id);
    }
    shader->setBool("isDiffuse", isDiff);
    shader->setInt("diffuseOne", diffOne);
    if (isDiffTwo)
    {
        glActiveTexture(GL_TEXTURE0  + diffTwo); // Active proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, textures[diffTwoX].id);
    }
    shader->setInt("diffuseTwo", diffTwo);
    if (isSpec)
    {
        glActiveTexture(GL_TEXTURE0  + specOne); // Active proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, textures[specOneX].id);
    }
    shader->setBool("isSpecular", isSpec);
    shader->setInt("specularOne", specOne);
    if (isBinorm)
    {
        glActiveTexture(GL_TEXTURE0  + binormOne); // Active proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, textures[binormOneX].id);
    }
    shader->setBool("isBinormal", isBinorm);
    shader->setInt("binormalOne", binormOne);
    shader->setFloat("shininess", 10.0f);
    shader->setVec3("viewPos", viewPos);
    shader->setFloat("opacity", opacity);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    cout  << "\n\n\tSampler IDs 2: diffOne " << diffOne 
    << " diffTwo " << diffTwo << " specOne " << specOne
    << " binormOne " << binormOne;
    dataIndex = glGetUniformBlockIndex(shader->Program, "itemData");   
    glUniformBlockBinding(shader->Program, dataIndex, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, VBO[1]);
    //! Pass the image indices and cube distances.
    glBufferData(GL_UNIFORM_BUFFER, quantity * sizeof(mat4), (void*)instanceArray,
    GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);    
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, dataIndex);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, VBO[1], 0, quantity * sizeof(mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    shader->setVec3("colordiff", vec3(1.0f, 1.0f, 1.0f));
    for (int x = 0; x < lights.size(); x++)
    {   
        stringstream ss;
        ss << x;
        shader->setVec3("pointLights[" + ss.str() + "].position", lights[x].position);
        shader->setVec3("pointLights[" + ss.str() + "].ambient", lights[x].ambient);
        shader->setVec3("pointLights[" + ss.str() + "].diffuse", lights[x].diffuse);
        shader->setVec3("pointLights[" + ss.str() + "].specular", lights[x].specular);
        shader->setFloat("pointLights[" + ss.str() + "].constant", lights[x].constant);
        shader->setFloat("pointLights[" + ss.str() + "].linear", lights[x].linear);
        shader->setFloat("pointLights[" + ss.str() + "].quadratic", lights[x].quadratic);
    }
    for (int x = 0; x < spotLights.size(); x++)
    {   
        stringstream ss;
        ss << x;
        shader->setVec3("spotLights[" + ss.str() + "].position", spotLights[x].position);
        shader->setVec3("spotLights[" + ss.str() + "].direction", spotLights[x].direction);
        shader->setFloat("spotLights[" + ss.str() + "].cutOff", spotLights[x].cutOff);
        shader->setFloat("spotLights[" + ss.str() + "].outerCutOff", spotLights[x].outerCutOff);
        shader->setVec3("spotLights[" + ss.str() + "].ambient", spotLights[x].ambient);
        shader->setVec3("spotLights[" + ss.str() + "].diffuse", spotLights[x].diffuse);
        shader->setVec3("spotLights[" + ss.str() + "].specular", spotLights[x].specular);
        shader->setFloat("spotLights[" + ss.str() + "].constant", spotLights[x].constant);
        shader->setFloat("spotLights[" + ss.str() + "].linear", spotLights[x].linear);
        shader->setFloat("spotLights[" + ss.str() + "].quadratic", spotLights[x].quadratic);
    }
    if (debug1)
    {
        cout << "\n\tCamera Position in MeshTex:  ";
        printVec3(viewPos);
        cout << "\n\tInstance quantity:  " << quantity << ".\n\n";
        UniformPrinter uniforms(shader->Program);
    }
    // Draw mesh.
    glDrawArraysInstanced(GL_TRIANGLES, 0, indexSize, quantity);
    if (debug1)
    {
        cout << "\n\n\t" << quantity << " instanced objects drawn.\n\n";
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO[1]);
}

void MeshTex::printVec3(vec3 vecVal)
{
    cout << "  3 Float Vector:  ";
    for  (int x = 0; x < 3; x++)
    {
        cout << vecVal[x] << ", ";
    }
}

void MeshTex::printMat4(mat4 matVal)
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


