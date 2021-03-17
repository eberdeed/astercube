/**********************************************************
 *   MeshVert:  A class that is derived from Mesh. This class
 *   is for untextured meshes.  This class isolates tha
 *   display of data from a 3d asset file using assimp.
 *   Adapted by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   and originally found on www.learnopengl.com.
 *   April 2020 San Diego, California USA
 * ********************************************************/

#include "../include/meshvert.h"

MeshVert::MeshVert()
{
    cout << "\n\n\tCreating MeshVert.\n\n";
    return;
}
    
//! Destroy array and buffers.
MeshVert::~MeshVert()
{
    cout << "\n\n\tDestroying MeshVert.\n\n";
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    if (instanced)
    {
        glDeleteBuffers(2, VBO);
    }
    else
    {
        glDeleteBuffers(1, &VBO[0]);
    }
    glDeleteBuffers(1, &EBO);
}

//!  Pass along data to be displayed from the Model class.
int MeshVert::setData(Vertex1 *vertices, GLuint *indices, vec3 color, int vertSize, int indexSize, bool instanced, int quantity, Shader *shader, int startIndex)
{
    this->vertices = vertices;
    this->indices = indices;
    this->colordiff = color;
    this->vertSize = vertSize;
    this->indexSize = indexSize;
    this->instanced = instanced;
    this->quantity = quantity;
    this->shader = shader;
    instanceArray = new mat4[quantity];
    setupMesh();
    diffOne = startIndex + dummyTex++ + startIndex; 
    diffTwo = startIndex + dummyTex++ + startIndex;
    specOne  = startIndex + dummyTex++ + startIndex;
    binormOne = startIndex + dummyTex++ + startIndex; 
    numDiff = 0;
    //dumpData();
    return (startIndex + 4);
}

void MeshVert::dumpData()
{
    cout << "\n\nVertices, Normals and TexCoords: \n\n";
    for (int x = 0; x < vertSize; x++)
    {
        cout << "\nVertex: " << vertices[x].Position[0] << ", " << vertices[x].Position[1] << ", " << vertices[x].Position[2] <<
        " Normal: " << vertices[x].Normal[0] << ", " << vertices[x].Normal[1] << ", " << vertices[x].Normal[2];
    }
    cout << "\n\n\nIndices: \n\n";
    for (int x = 0; x < indexSize; x++)
    {
        cout << indices[x];
        if (((x + 1) % 25) == 0)
        {
            cout << "\n\n";
        }
    }
    cout << "\n\n";
}

//! Allocate array and buffers.
void MeshVert::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    if (instanced)
    {
        glGenBuffers(2, VBO);
    }
    else
    {
        glGenBuffers(1, &VBO[0]);
    }
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex1), vertices, GL_STATIC_DRAW);  
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(GLuint), indices, GL_STATIC_DRAW);
    
    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (GLvoid*)0);
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); 
    if (instanced)
    {
        //! Uniform buffer to feed the uniform.
        dataIndex = glGetUniformBlockIndex(shader->Program, "itemData");   
        glUniformBlockBinding(shader->Program, dataIndex, 0);
        glBindBuffer(GL_UNIFORM_BUFFER, VBO[1]);
        glBufferData(GL_UNIFORM_BUFFER, quantity * sizeof(mat4), (void*)instanceArray,
        GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindVertexArray(0);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, dataIndex);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, VBO[1], 0, quantity * sizeof(mat4));
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}  

//! Draw object.
void MeshVert::Draw(mat4 view, mat4 projection, mat4 model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, bool diffOnly, float gamma) 
{
    
    shader->setInt("numDiffuse", numDiff);
    shader->setBool("isDiffuse", isDiff);
    shader->setInt("diffuseOne", diffOne);
    shader->setInt("diffuseTwo", diffTwo);
    shader->setBool("isSpecular", isSpec);
    shader->setInt("specularOne", specOne);
    shader->setBool("isBinormal", isBinorm);
    shader->setInt("binormalOne", binormOne);
    shader->setBool("diffOnly", diffOnly);
    shader->setFloat("gamma", gamma);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", model);
    shader->setVec3("viewPos", viewPos);
    //! No texture present.
    shader->setBool("isDiffuse", false);
    shader->setInt("numDiffuse", 0);
    shader->setFloat("shininess", 1.0f);
    shader->setVec3("colordiff", colordiff);
    shader->setFloat("opacity", opacity);
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
        cout << "\n\n\tOpacity:  " << opacity << "  Color Vector:  " 
        << colordiff.x << ", " << colordiff.y << ", " 
        << colordiff.z << "\n\n";
    }
    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

//! Draw object instanced.
void MeshVert::DrawInstanced( mat4 view, mat4 projection, vector<mat4>model, vector<PointLight>lights, vector<SpotLight>spotLights, vec3 viewPos, bool diffOnly, float gamma) 
{
    shader->setInt("numDiffuse", numDiff);
    shader->setBool("isDiffuse", isDiff);
    shader->setInt("diffuseOne", diffOne);
    shader->setInt("diffuseTwo", diffTwo);
    shader->setBool("isSpecular", isSpec);
    shader->setInt("specularOne", specOne);
    shader->setBool("isBinormal", isBinorm);
    shader->setInt("binormalOne", binormOne);
    for (int x = 0; x < quantity; x++)
    {
        instanceArray[x] = model[x];
    }
    
    shader->setBool("diffOnly", diffOnly);
    shader->setFloat("gamma", gamma);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    glBindBuffer(GL_UNIFORM_BUFFER, VBO[1]);
    //! Pass the image indices and cube distances.
    glBufferSubData(GL_UNIFORM_BUFFER, 0, quantity * sizeof(mat4), (void*) modelData); 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);    
    shader->setVec3("viewPos", viewPos);
    //! No texture present.
    shader->setBool("isDiffuse", false);
    shader->setInt("numDiffuse", 0);
    shader->setFloat("shininess", 1.0f);
    shader->setVec3("colordiff", colordiff);
    shader->setFloat("opacity", opacity);
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
    cout << "\n\n\tOpacity:  " << opacity << "  Color Vector:  " 
    << colordiff.x << ", " << colordiff.y << ", " 
    << colordiff.z << "\n\n";
    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, indices, quantity);
    glBindVertexArray(0);
}
