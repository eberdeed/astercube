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

#include "../include/objects.h"

Objects::Objects()
{
    cout << "\n\n\tCreating Objects.\n\n";
    generator = mt19937(rd());
}
Objects::~Objects()
{
    cout << "\n\n\tDestorying Objects.\n\n";
    delete shader;
}
void Objects::setScale(float value)
{
    //! The size of the asteroid field;
    cubescale = value;
}
void Objects::setSkyBox(unsigned int skybox)
{
    skyboxTex = skybox;
}
void Objects::debug()
{
    cout << "\n\n\tBlender Model Information\n\n";
    if (debug1)
    {
        for (int x = 0; x < modelinfo.size(); x++)
        {
            cout << "\n\t" << modelinfo[x].path << "\n";
            printMat4(modelinfo[x].model);
            printVec3(modelinfo[x].location);
            cout << "\t" << modelinfo[x].gamma << "\n";
            cout << "\t" << modelinfo[x].idval << "\n";
        }
    }
}
void Objects::initObjects()
{
        createAsteroids(AMOUNT);
        shader = new Shader();
        shader->initShader(vertexShader, fragmentShader, "glastercube.bin");
        cout << "\n\n\tShader created.\n\n";
        figure = new Model(modelinfo, QUANTITY, shader, 2);
        debug();
}

void Objects::drawObjects(mat4 model, mat4 view, mat4 projection, vec3 viewPos)
{
        
        mat4 loc1, loc2;
        loc1 = scale(model, vec3(4.0f, 4.0f, 4.0f));
        calcPosition(loc1, AMOUNT);
        modelData.clear();
        for (int x = 0; x < QUANTITY * AMOUNT; x++)
        {
            modelData.push_back(tmpLocs[x].objMatrices);
            if (debug1)
            {
                cout << "\n\tData for asteroid " << x << " in the Objects class.";
                printMat4(modelData[x]);
            }
        }
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex); 
        shader->setInt("SkyBoxOne", 1);
        figure->DrawInstanced(view, projection, modelinfo, modelData, lights, spotLights, viewPos);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
vec3 Objects::getDirection(vec3 viewer, vec3 viewed)
{
    vec3 tmpval = normalize(viewed - viewer);
    return(tmpval);
}

void Objects::createAsteroids(int value)
{
    cout << "\n\n\tIn createAsteroids().\n\n";
    //! The limits of the asteroid field.
    minx = miny = minz = -cubescale;
    maxx = maxy = maxz = cubescale;
    //! The rotation and location.
    mat4 matObj;
    vec3 tmpvec;
    //! The location.
    vec4 Loc;
    //! The cube root of the total number of asteroids as an integer.
    GLuint limit = pow((float)(QUANTITY * value), (1.0f / 3.0f));
    //! The upper bound for asteroid position variation.
    GLfloat offset = cubescale / (((float)QUANTITY * value) / 2.0f);
    //! The maximum  of asteroid size.
    GLfloat multi = 1.0f;
    //! The total asteroid count.
    int count = 0;
    cout << "\n\n\tGenerating asteroid locations for " << QUANTITY * value << " asteroids.\n\n";
    //! Generate a large list of semi-random model transformation matrices
    for(GLuint i = 0; i < limit; i++)
    {
        for(GLuint j = 0; j < limit; j++)
        {
            for (GLuint k = 0; k < limit; k++)
            {
                //! 1. Translation: Randomly displace in a cube of size, with each dimension of value (0, offset).
                GLfloat x = minx + (((cubescale * 2.0f) / limit) * randomFloats(generator) * limit);
                GLfloat y = miny + (((cubescale * 2.0f) / limit) * randomFloats(generator) * limit);
                GLfloat z = minz + (((cubescale * 2.0f) / limit) * randomFloats(generator) * limit);
                //! Keep track a min and max values for the three dimensions.
                if (x < minx)
                {
                    x = minx;
                }
                if (y < miny)
                {
                    y = miny;
                }
                if (z < minz)
                {
                    z = minz;
                }
                if (x > maxx)
                {
                    x = maxx;
                }
                if (y > maxy)
                {
                    y = maxy;
                }
                if (z > maxz)
                {
                    z = maxz;
                }
                //! Create and save the location.
                if ((i == 0) && (k == 0) && (j ==  0))
                {
                    tmpvec = vec3(0, 0, 0);
                }
                else
                {
                    tmpvec = vec3(x, y, z);
                }
                if (debug1)
                {
                    cout << "\n\tAsteroid " << count + 1
                    << " location ";
                    printVec3(tmpvec);
                }
                //! Translate the 4x4 matrix.
                mat4 model = mat4(1.0f);
                model = translate(model, tmpvec);
                if (debug1)
                {
                    printMat4(model);
                }
                
                //! 2. Scale: Scale between 1.0 and multi * 1.0f
                GLfloat scaler = (multi * randomFloats(generator)) + 1.0;
                if (debug1)
                {
                    cout << "\n\tScaler:  " << scaler;
                }
                if (scaler > (multi + 2.0))
                {
                    cout << "\nScale error:  " << multi << "\n";
                    exit(1);
                }
                //! Scale the 4x4 matrix.
                model = scale(model, vec3(scaler, scaler, scaler));
                if (debug1)
                {
                    printMat4(model);
                }
                velocity = randVec();
                velocity *= randomFloats(generator) * 0.03;
                float sign = 1.0f;
                if (randomFloats(generator) > 0.5f)
                {
                    sign = -1.0f;
                }
                rotation = sign * randomFloats(generator) * (pi180 / 90.0f); // 2 degrees.
                //! 3. Rotation: add random rotation around a rotation axis vector
                GLfloat degAngle = (GLfloat)(randomFloats(generator) * 360.0f);
                GLfloat rotAngle = degAngle * (3.14159f / 180.0f);
                vec3 axis = normalize(randVec());
                //! Rotate the 4x4 matrix.
                model = rotate(model, rotAngle, axis);
                tmpLocs[count].objMatrices = model;
                tmpLocs[count].axis = axis;
                tmpLocs[count].objLocs = tmpvec;
                tmpLocs[count].radii = scaler;
                tmpLocs[count].rotation = 0.0f; 
                tmpLocs[count].rotVel = rotation;
                tmpLocs[count++].velocity = velocity;
                if (debug1)
                {
                    cout << "\n\tFinal product.\n";
                    cout << "\n\tLocation: ";
                    printVec3(tmpvec);
                    cout << "\n\tRotation axis: ";
                    printVec3(axis);
                    cout << "\n\tSize: " << scaler;
                    cout << "\n\tRotation Velocity: " << rotation;
                    cout << "\n\tLinear Velocity: ";
                    printVec3(velocity);
                    cout << "\n\tFinal Affine matrix: ";
                    printMat4(model);
                }
                
            }
        }
    }
    calcPosition(mat4(1.0f), value);
    cout << "\n\n\tCreated asteroid locations.\n\n";
   for (int x = 0; x < value; x++)
    {
        //! The translation or location information.
        //! The file name.
        item.path = asteroids[x];
        //! The position and orientation matrix.
        item.model = tmpLocs[x].objMatrices;
        item.gamma = 2.3f;
        item.location = tmpLocs[x].objLocs;
        //! Tack it onto the vector.
        modelinfo.push_back(item);
    }
}

void Objects::calcPosition(mat4 model, int value)
{
    //! Distribute the asteroid location, scale and rotation values among the three asteroids.
    //! each value of matLocs is one type of asteroid field value.
    instanceData = new float[(int) QUANTITY * value * 16];
    int count = 1;
    mat4 matpos;
    vec4 vecpos;
    vec3 tmpvec;
    float scaler;
    float rotation;
    for (int q = 0; q < value; q++)
    {
        for (int y  = 0; y < (int) QUANTITY; y++)
        {
            tmpLocs[(q * QUANTITY) + y].objLocs += tmpLocs[(q * QUANTITY) + y].velocity;
            if ((tmpLocs[(q * QUANTITY) + y].objLocs.x < minx) || (tmpLocs[(q * QUANTITY) + y].objLocs.x > maxx)
            || (tmpLocs[(q * QUANTITY) + y].objLocs.y < miny) || (tmpLocs[(q * QUANTITY) + y].objLocs.y > maxy)
            || (tmpLocs[(q * QUANTITY) + y].objLocs.z < minz) || (tmpLocs[(q * QUANTITY) + y].objLocs.z > maxz))
            {
                tmpLocs[(q * QUANTITY) + y].objLocs *= -1.0f;
            }
            for (int x = 0; x < (q * QUANTITY) + y; x++)
            {
                //! Collision:  switch velocities and reverse rotations.
                if (distance(tmpLocs[(q * QUANTITY) + y].objLocs, tmpLocs[x].objLocs) < (tmpLocs[(q * QUANTITY) + y].radii + tmpLocs[x].radii))
                {
                    tmpvec = tmpLocs[q * QUANTITY + y].velocity;
                    tmpLocs[q * QUANTITY + y].velocity = tmpLocs[x].velocity;
                    tmpLocs[x].velocity = tmpvec;
                    tmpLocs[q * QUANTITY + y].rotVel *= -1.0f;
                    tmpLocs[x].rotVel *= -1.0f;
                }
            }
            tmpLocs[q * QUANTITY + y].rotation += tmpLocs[q * QUANTITY + y].rotVel;
            tmpLocs[q * QUANTITY + y].rotation = fmod(tmpLocs[q * QUANTITY + y].rotation, pi360);
            scaler = tmpLocs[q * QUANTITY + y].radii;
            matpos = mat4(1.0f);
            matpos = translate(matpos, tmpLocs[(q * QUANTITY) + y].objLocs); 
            matpos = scale(matpos, vec3(scaler, scaler, scaler));
            matpos = rotate(matpos, tmpLocs[(q * QUANTITY) + y].rotation, tmpLocs[(q * QUANTITY) + y].axis);
            tmpLocs[(q * QUANTITY) + y].objMatrices = matpos;
            if (debug1)
            {
                cout << "\n\tData for asteroid:  Location " << q << "\n";
                printVec3(tmpLocs[(q * QUANTITY) + y].objLocs);
                cout << "\n\tAffine matrix ";
                printMat4(tmpLocs[(q * QUANTITY) + y].objMatrices);
                cout << "\n\tRotation: " << tmpLocs[q * QUANTITY + y].rotation;
            }
        }
    }
    if (debug1)
    {
        debug();
        cout << "\n\n\tAsteroid initialization ended.\n\n";
    }
}

void Objects::printVec3(vec3 vecVal)
{
    cout << "  3 Float Vector:  ";
    for  (int x = 0; x < 3; x++)
    {
        cout << vecVal[x] << ", ";
    }
}

void Objects::printMat4(mat4 matVal)
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

vec3 Objects::randVec()
{
    vec3 value;
    float x, y, z, sign = 1.0f;
    if (randomFloats(generator) > 0.5)
    {
        sign *= -1.0f;
    }
    x = sign * randomFloats(generator);
    if (randomFloats(generator) > 0.5)
    {
        sign *= -1.0f;
    }
    y = sign * randomFloats(generator);
    if (randomFloats(generator) > 0.5)
    {
        sign *= -1.0f;
    }
    z = sign * randomFloats(generator);
    value = vec3(x, y, z);
    return value;
}
