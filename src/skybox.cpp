/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2020  <copyright holder> <email>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "../include/skybox.h"

SkyBox::SkyBox()
{
    cout << "\n\n\tCreating SkyBox.\n\n";
}
SkyBox::~SkyBox()
{
    cout << "\n\n\tDestroying SkyBox.\n\n";
    delete image;
    delete skyboxShader;
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteVertexArrays(1, &skyboxVAO);
}
void SkyBox::setScale(float value)
{
    size = value * 2.0f;
}
unsigned int SkyBox::getSkyBox()
{
    return skyboxTex;
}

void SkyBox::initSkyBox()
{
    skyboxShader = new Shader();
    skyboxShader->initShader(vertexShader, fragmentShader, "supercubeskybox.bin");
    cout << "\n\n\tCreated skybox shader.\n\n";
    image = new CreateImage();
    image->createSkyBoxTex(skyboxTex, skybox);
    cuby = new AsterObject();
    skycube = cuby->genCube(size, false, false, false);
    //debug();
    //! Set up the skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), skycube, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
void SkyBox::debug()
{
    for (int x = 0; x < 36; x++)
    {
        cout << "\n\tVertex " << x + 1 << ":  " << skycube[x * 3]
        << ", " << skycube[x * 3 + 1]
        << ", " << skycube[x * 3 + 2];
    }
}

void SkyBox::drawSkyBox(mat4 model, mat4 view, mat4 projection)
{
        glDisable(GL_CULL_FACE);
        skyboxShader->Use();
        skyboxShader->setMat4("view", view);
        skyboxShader->setMat4("projection", projection);
        skyboxShader->setMat4("model", model);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        skyboxShader->setInt("skybox", 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glBindVertexArray(0);
    
}
