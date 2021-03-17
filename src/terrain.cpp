/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2021  <copyright holder> <email>
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

#include "../include/terrain.h"

Terrain::Terrain()
{
    cout << "\n\n\tCreating Terrain.\n\n";
}

Terrain::~Terrain()
{
    cout << "\n\n\tDestroying Terrain.\n\n";
    delete image;
    delete floorShader;
    glDeleteBuffers(1, &floorVBO);
    glDeleteVertexArrays(1, &floorVAO);

}
void Terrain::debug()
{
    for (int x = 0; x < 6; x++)
    {
        cout << "\n\tVertex " << x + 1 << ":  " << floorVertices[x * 5]
        << ", " << floorVertices[x * 5 + 1]
        << ", " << floorVertices[x * 5 + 2]
        << "\tTexture " <<  floorVertices[x * 5 + 3]
        << ", " << floorVertices[x * 5 + 4];
    }
    for (int x = 0; x < 600; x++)
    {
        cout << "\n\tVertex " << x + 1 << ":  " << floorLayout[x * 5]
        << ", " << floorLayout[x * 5 + 1]
        << ", " << floorLayout[x * 5 + 2]
        << "\tTexture " <<  floorLayout[x * 5 + 3]
        << ", " << floorLayout[x * 5 + 4];
    }
}

void Terrain::setScale(float value)
{
    cout << "\n\n\tIn setScale for Terrain.\n\n";
    size = value;
    for (int x = 0; x < 6; x++)
    {
        floorVertices[x * 5] *= value / 10.0f;
        floorVertices[x * 5 + 1] *= value / 10.0f;
        floorVertices[x * 5 + 2] *= value / 10.0f;
    }
    adjust = rotate(adjust, acos(-1.0f) / 2.0f, vec3(1.0f, 0.0f, 0.0f));
    adjust = translate(adjust, vec3(0.0f, 0.0f, -value + (value * 0.01f)));
    floorLayout = new float[3000];
    int count = 0;
    for (int x = -5; x < 5; x++)
    {
        for (int y = -5; y < 5; y++)
        {
            for (int z = 0; z < 6; z++)
            {
                floorLayout[count++] = floorVertices[z * 5] + ((size / 5.0f) * x) + 100.0f;
                floorLayout[count++] = floorVertices[z * 5 + 1] + ((size / 5.0f) * y) + 100.0f;
                floorLayout[count++] = floorVertices[z * 5 + 2];
                floorLayout[count++] = floorVertices[z * 5 + 3];
                floorLayout[count++] = floorVertices[z * 5 + 4];
            }
        }
    }
    if (debug1)
    {
        cout << "\n\n\tFloor Layout Total:  " << count << "\n\n";
        debug();
    }
}
void Terrain::initTerrain()
{
    floorShader = new Shader();
    floorShader->initShader(string("../../openglresources/shaders/floor.vs"),
    string("../../openglresources/shaders/floor.frag"),
    string("supercubefloor.bin"));
    glGenTextures(1, &floorTex);
    image = new CreateImage();
    image->setImage("../../openglresources/objects/images/dirt.jpg");
    floorTex = image->textureObject();
    cout << "\n\n\tCreating floor vertex buffer.\n\n";
    // Generate the sky box.
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, 3000 * sizeof(float), floorLayout, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Terrain::drawTerrain(mat4 model, mat4 view, mat4 projection)
{
        floorShader->Use();
        model = model * adjust;
        floorShader->setMat4("view", view);
        floorShader->setMat4("projection", projection);
        floorShader->setMat4("model", model);
        glBindVertexArray(floorVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, floorTex);
        floorShader->setInt("ground", 1);
        glDrawArrays(GL_TRIANGLES, 0, 600);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
}    
