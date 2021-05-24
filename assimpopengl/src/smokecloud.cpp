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

#include "../include/smokecloud.h"

SmokeCloud::SmokeCloud(Shader *shader)
{
    cout << "\n\n\tCreating SmokeCloud.\n\n";
    this->shader = shader;
    tdata = chrono::system_clock::now();
    // Fill in particle data array
    srand ((long int)chrono::system_clock::to_time_t(tdata));
    // Create the data.
    generator = mt19937(rd());
    scaler = mat4(1.0);
    scaler = scale(scaler, vec3((float)SCALE, (float)SCALE, (float)SCALE));
    initFire();
}

SmokeCloud::~SmokeCloud()
{
    cout << "\n\n\tDestroying SmokeCloud.\n\n";
    delete shader;
    glDeleteBuffers(1, &VAO);
   
}

void SmokeCloud::debug()
{
        for (int x = 0; x < NUM_PARTICLES; x++)
    {
        cout << "\n\tDisplacement phi:  " 
        << ", " << particles[x].x << ", "
        << "\tDisplacement rho:  " << particles[x].y << ", "
        << "\tRadius:  " << particles[x].z
        << "\tLifetime:  " << particles[x].w;
        printVec4(particles[x]);
    }
}

void SmokeCloud::initFire()
{
    float r, theta;
    cout << "\n\n\tIn initSound.\n\n";
    for (int i = 0; i < NUM_PARTICLES; i++ )
    {
        theta = randomFloats(generator) * pi360;
        //! Spherical coordinates.
        //! phi
        particles[i].x = fmod(theta, pi360);
        theta = randomFloats(generator) * pi360;
        //! rho
        particles[i].y = fmod(theta, pi360);
        r = (randomFloats(generator) / 2.0f) + 0.3f;
        //! radius
        particles[i].z = glm::clamp(r, 0.3f, 1.0f);
        // Lifetime of particle (200 frames).
        particles[i].w = randomFloats(generator) * (float) LIFETIME;
    }
    if (debug1)
    {
        debug();
    }
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    if (debug1)
    {
        cout << "\n\n\tColor: " << color.r << ", " << color.g << ", " 
        << color.b << ", " <<  color.a;
    }
    defineObjects();
}

void SmokeCloud::drawCloud(mat4 model, mat4 view, float interval)
{
    if (debug1)
    {
        cout << "\n\n\tProcessing interval:  " << interval << ".\n\n";
        cout << "\n\tLocation Matrix:\n";
        printMat4(model);
        cout << "\n\tView Matrix:\n";
        printMat4(view);
    }
    glBindVertexArray(VAO);
    shader->Use();
    shader->setFloat("u_time", interval);
    shader->setVec4("u_color", color);
    shader->setMat4("scaler", scaler);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
    glBindVertexArray(0);
}


void SmokeCloud::logSDLError(ostream &os, const string &msg)
{
    os << "\n\n\t" << msg << " error: " << SDL_GetError() << "\n\n";
}

void SmokeCloud::defineObjects()
{
    //Create vertex buffer.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLES * sizeof(vec4), particles, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}    


void SmokeCloud::printVec3(vec3 vecVal)
{
    cout << "  3 Float Vector:  ";
    for  (int x = 0; x < 3; x++)
    {
        cout << vecVal[x] << ", ";
    }
}
void SmokeCloud::printVec4(vec4 vecVal)
{
    cout << "  4 Float Vector:  ";
    for  (int x = 0; x < 4; x++)
    {
        cout << vecVal[x] << ", ";
    }
}

void SmokeCloud::printMat4(mat4 matVal)
{
    cout << "  4x4 Matrix\n\t";
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            cout << fixed << setprecision(4) << setw(8) << matVal[y][x] << ", ";
        }
        cout << "\n\t";
    }
}
