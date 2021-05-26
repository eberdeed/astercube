/**********************************************************
 *   Mesh:  This class isolates the display of data from 
 *   a 3d asset file using assimp.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>,
 *   it was originally found on www.learnopengl.com.
 *   April 2020 San Diego, California USA
 *********************************************************/
#include "../include/mesh.h"

Mesh::Mesh()
{
    cout << "\n\n\tCreating Mesh.\n\n";
    return;
}
    
Mesh::~Mesh()
{
    cout << "\n\n\tDestroying Mesh.\n\n";
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO[0]);
    glDeleteBuffers(1, &EBO);
}
void Mesh::debug(mat4 *modelData)
{
    for (int y = 0; y < quantity; y++)
    {
        cout << "\n\tObject:  " << y << " in the Mesh class.";
        printMat4(modelData[y]);
    }
}

void Mesh::uniforms()
{
    cout << "\n\tisSpec " << isSpec << " isBinorm " << isBinorm;
    cout << "\n\tdiffOne " << diffOne << " diffTwo " << diffTwo;
    cout << "\n\tspecOne " << specOne << " binormOne " << binormOne;
    cout << "\n\tdiffOneX " << diffOneX << " diffTwoX " << diffTwoX;
    cout << "\n\tspecOneX " << specOneX << " binormOneX " << binormOneX;
    cout << "\n\tnumDiff " << numDiff << " opacity " << opacity;
    cout << "\n\tgamma " << gamma;
    cout << "\n\tcolordiff ";
    printVec3(colordiff);        
}

// Pass along data from the model class to be drawn here.
int Mesh::setData(Vertex *vertices, GLuint *indices, vector<Texture>textures, vec3 colordiff, 
int vertSize, int indexSize, bool instanced, int quantity, Shader *shader, Shader *explodeShader, 
int startIndex)
{
    currVal = 0;
    maxVal = indexSize / 3;
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->colordiff = colordiff;
    this->vertSize = vertSize;
    this->indexSize = indexSize;
    this->instanced = instanced;
    this->quantity = quantity;
    this->shader = shader;
    this->explodeShader = explodeShader;
    maxVal = indexSize / 3;
    if ((instanced) && (quantity > 0))
    {
        instanceArray = new mat4[quantity];
    }
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
    //! Create individual chunks for the explosion.
    //! Each chunk has chunkSize * 3 vertices (chunkSize triangles).
    cout << "\n\tReached creating chunks.\n";
    while(currVal < maxVal)
    {
        chunkSize = (rand() % CHUNK_SIZE) + 1;
        currVal += chunkSize;
        if (debug1)
        {
            cout << "\n\tChunk Size " << chunkSize  * 3 << " Current Value " << currVal
            << " with Maximum Value " << maxVal << " and Index Size " << indexSize;
        }
        if (currVal > maxVal)
        {
            chunkSize = abs(maxVal - currVal);
        }
        chunks.push_back(chunkSize * 3);
        chunkCount++;
    };
    numDiff = 0;
    //! Bind appropriate textures
    //! Here we allow for the three types of textures: Diffuse, specular and binormal or bumpmap.
    for( int x = 0; x < textures.size(); x++)
    {
        //! Texture present.
        if (debug1)
        {
            cout << "\n\t Texture Path:  " << textures[x].path << "  Texture Type:  " << textures[x].type << "  Index:  " << startIndex + x;
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
       isDiffTwo = false;
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
    if (!isSpec && !isBinorm && isDiff)
    {
        diffOnly = true;
    }
    else
    {
        diffOnly = false;
    }
    setupMesh();
    if (debug1)
    {
        dumpData();
    }
    if (true)
    {
        uniforms();
    }
    return (startIndex + 4);
}

//! For debugging.
void Mesh::dumpData()
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
void Mesh::setupMesh()
{
    cout << "\n\n\tIn setupMesh().\n\n";
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}  


void Mesh::setType(string val)
{
    type = val;
}

string Mesh::getType()
{
    return type;
}

//! Draw the object.
void Mesh::Draw(mat4 view, mat4 projection, mat4 model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, bool diffOnly, float gamma) 
{
    shader->Use();
    start = true;
    glBindVertexArray(VAO);
    opacity = 1.0f;
    if(debug1)
    {
        cout  << "\n\n\tSampler IDs Single: diffOne " << diffOne 
        << " diffTwo " << diffTwo << " specOne " << specOne
        << " binormOne " << binormOne;
    }
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
        glActiveTexture(GL_TEXTURE0  + specOne); 
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
    shader->setVec3("colordiff", colordiff);
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
        cout << "\n\tModel in Mesh:  ";
        printMat4(model);
        cout << "\n\tCamera Position in Mesh:  ";
        printVec3(viewPos);
    }
    // Draw mesh
    glDrawArrays(GL_TRIANGLES, 0, indexSize);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

//! Draw the object.
void Mesh::DrawInstanced(mat4 view, mat4 projection, vector<mat4>model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, bool diffOnly, float gamma, unsigned int total) 
{
    shader->Use();
    start = true;
    for (int x = 0; x < total; x++)
    {
        instanceArray[x] = model[x];
    }
    if (debug1)
    {
        debug(instanceArray);
        cout << "\n\n\tIs instanced:  " << instanced << "\n\n";
    }
    glBindVertexArray(VAO);
    opacity = 1.0f;
    
    //! Bind appropriate textures
    //! Here we allow for the three types of textures: Diffuse, specular and binormal or bumpmap.
    if (debug1)
    {
        cout  << "\n\n\tSampler IDs Instanced: diffOne " << diffOne 
        << " diffTwo " << diffTwo << " specOne " << specOne
        << " binormOne " << binormOne;
    }
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
    shader->setVec3("colordiff", colordiff);
    dataIndex = glGetUniformBlockIndex(shader->Program, "itemData");   
    glUniformBlockBinding(shader->Program, dataIndex, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, VBO[1]);
    //! Pass the image indices and cube distances.
    glBufferData(GL_UNIFORM_BUFFER, total * sizeof(mat4), (void*)instanceArray,
    GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);    
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, dataIndex);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, VBO[1], 0, quantity * sizeof(mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
        cout << "\n\tCamera Position in Mesh:  ";
        printVec3(viewPos);
        cout << "\n\tInstance quantity:  " << quantity << ".\n\n";
        UniformPrinter uniforms(shader->Program);
    }
    // Draw mesh.
    glDrawArraysInstanced(GL_TRIANGLES, 0, indexSize, total);
    if (debug1)
    {
        cout << "\n\n\t" << quantity << " instanced objects drawn.\n\n";
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO[1]);
}

void Mesh::explosion(mat4 model, mat4 view, mat4 projection, float timeVal)
{
    //cout << "\n\tExplosion in Mesh.";
    start = false;
    //cout << "\n\tCloud drawn.";
    currVal = 0;
    mat4 chunkArray[chunkCount];
    for (int x = 0; x < chunkCount; x++)
    {
        
        chunkLoc = mat4(1.0f);
        for (int y = 0; y < 3; y++)
        {
            vert[y] = verticesIndexed[currVal].Normal[y] * timeVal * 0.1f;
        }
        chunkLoc = translate(model, vert);
        chunkArray[x] = chunkLoc;
        currVal += chunks[x];
        if (debug1)
        {
            cout << "\n\tIndex " << x << " currVal " << currVal << " chunks " 
            << chunks[x] << " indexSize " << indexSize;
            printVec3(vert);
            printMat4(chunkLoc);
            cout << "\n";
        }

    }
    explodeShader->Use();
    glBindVertexArray(VAO);
    dataIndex = glGetUniformBlockIndex(explodeShader->Program, "itemData");   
    //cout << "\n\tglGetUniformBlockIndex() " << getError();
    glUniformBlockBinding(explodeShader->Program, dataIndex, 0);
    //cout << "\n\tglUniformBlockBinding() " << getError();
    int param;
    glGetActiveUniformBlockiv(explodeShader->Program, dataIndex, GL_UNIFORM_BLOCK_BINDING, &param);
    //cout << "\n\tUniform block binding for itemData is " << param 
    //<< " with dataIndex " << dataIndex << ".\n";
    glBindBuffer(GL_UNIFORM_BUFFER, VBO[1]);
    //cout << "\n\tglBindBuffer() " << getError();
    //! Pass the vertex location information.
    glBufferData(GL_UNIFORM_BUFFER, chunkCount * sizeof(mat4), (void*)chunkArray, GL_DYNAMIC_DRAW);
    //cout << "\n\tglBufferData() " << getError();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);    
    //cout << "\n\tglBindBuffer() " << getError();
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, dataIndex);
    //cout << "\n\tglBindBufferBase() " << getError();
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, VBO[1], 0, chunkCount * sizeof(mat4));
    //cout << "\n\tglBindBufferRange() " << getError();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    //cout << "\n\tglBindBuffer() " << getError();
    if (debug1)
    {
        UniformPrinter printer(explodeShader->Program);
    }
    //cout << "\n\tTime Value " << timeVal;
    explodeShader->setMat4("model", model);
    explodeShader->setMat4("view", view);
    explodeShader->setMat4("projection", projection);
    explodeShader->setFloat("timeVal", timeVal);
    if (debug1)
    {
        cout << "\n\tExplosion Matrix: ";
        printMat4(model);
    }
    int start = 0;
    for (int x = 0; x < chunkCount; x++)
    {
        explodeShader->setInt("index", x);
        glDrawArrays(GL_TRIANGLES, start, chunks[x]);
        start += chunks[x];
    }
    
    glBindVertexArray(0);
}
        
    
void Mesh::printVec3(vec3 vecVal)
{
    cout << "  3 Float Vector:  ";
    for  (int x = 0; x < 3; x++)
    {
        cout << vecVal[x] << ", ";
    }
}

void Mesh::printMat4(mat4 matVal)
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


string Mesh::getError()
{
    GLenum data = glGetError();
    int dataVal = (int) data;
    string answer = "";
    switch(dataVal)
    {
        case GL_NO_ERROR:
            answer = "GL_NO_ERROR";
            break;
        case GL_INVALID_ENUM:
            answer = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            answer = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            answer = "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            answer = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            answer = "GL_OUT_OF_MEMORY";
            break;
        default:
            answer = "Unknown value.";
            break;
    }
    return answer;
}
