/**************************************************************
 * AsterObject:  A class that creates a cube in a variety 
 * of ways.
 * Create 08/2020 by Edward C. Eberle <eberdeed@eberdeed.net>
 * San Diego, California USA
 *************************************************************/

#include "../include/asterobject.h"

AsterObject::AsterObject()
{
    cout << "\n\n\tCreating AsterObject.\n\n";
    genMatrices();
}

AsterObject::~AsterObject()
{
    cout << "\n\n\tDestroying AsterObject.\n\n";
}

float *AsterObject::genCube(float scale, bool colored, bool normaled, bool textured)
{
    for (int x = 0; x < NUM_VERTICES * 3; x++)
    {
        calcCube[x] *= scale;
    }
    if (colored && textured && normaled)
    {
        cubeptr = new float[NUM_VERTICES * 11];
    }
    else if ((!textured) && (colored) && (normaled))
    {
        cubeptr = new float[NUM_VERTICES * 9];
    }
    else if ((!colored) && (textured) && (normaled))
    {
        cubeptr = new float[NUM_VERTICES * 8];
    }
    else if ((!normaled) && (textured) && (colored))
    {
        cubeptr = new float[NUM_VERTICES * 8];
    }
    else if ((!colored) && (!textured) && (normaled))
    {
        cubeptr = new float[NUM_VERTICES * 6];
    }
    else if ((!normaled) && (!textured) && (colored))
    {
        cubeptr = new float[NUM_VERTICES * 6];
    }
    else if ((!normaled) && (!colored) && (textured))
    {
        cubeptr = new float[NUM_VERTICES * 5];
    }
    else
    {
        cubeptr = new float[NUM_VERTICES * 3];
    }
    for (int x = 0; x < NUM_VERTICES; x++)
    {
        if (colored && textured && normaled)
        {
            cubeptr[x * 11] = calcCube[x * 3];
            cubeptr[(x * 11) + 1] = calcCube[(x * 3) + 1];
            cubeptr[(x * 11) + 2] = calcCube[(x * 3) + 2];
            cubeptr[(x * 11) + 3] = calcColor[x * 3];
            cubeptr[(x * 11) + 4] = calcColor[(x * 3) + 1];
            cubeptr[(x * 11) + 5] = calcColor[(x * 3) + 2];
            cubeptr[(x * 11) + 6] = calcNorm[x * 3];
            cubeptr[(x * 11) + 7] = calcNorm[(x * 3) + 1];
            cubeptr[(x * 11) + 8] = calcNorm[(x * 3) + 2];
            cubeptr[(x * 11) + 9] = calcTex[x * 2];
            cubeptr[(x * 11) + 10] = calcTex[x * 2 + 1];
        }
        else if ((!textured) && (colored) && (normaled))
        {
            cubeptr[x * 9] = calcCube[x * 3];
            cubeptr[x * 9 + 1] = calcCube[x * 3 + 1];
            cubeptr[x * 9 + 2] = calcCube[x * 3 + 2];
            cubeptr[x * 9 + 3] = calcColor[x * 3];
            cubeptr[x * 9 + 4] = calcColor[x * 3 + 1];
            cubeptr[x * 9 + 5] = calcColor[x * 3 + 2];
            cubeptr[x * 9 + 6] = calcNorm[x * 3];
            cubeptr[x * 9 + 7] = calcNorm[x * 3 + 1];
            cubeptr[x * 9 + 8] = calcNorm[x * 3 + 2];
        }
        else if ((!colored) && (textured) && (normaled))
        {
            cubeptr[x * 8] = calcCube[x * 3];
            cubeptr[x * 8 + 1] = calcCube[x * 3 + 1];
            cubeptr[x * 8 + 2] = calcCube[x * 3 + 2];
            cubeptr[x * 8 + 3] = calcNorm[x * 3];
            cubeptr[x * 8 + 4] = calcNorm[x * 3 + 1];
            cubeptr[x * 8 + 5] = calcNorm[x * 3 + 2];
            cubeptr[x * 8 + 6] = calcTex[x * 2];
            cubeptr[x * 8 + 7] = calcTex[x * 2 + 1];
        }
        else if ((!normaled) && (textured) && (colored))
        {
            cubeptr[x * 8] = calcCube[x * 3];
            cubeptr[x * 8 + 1] = calcCube[x * 3 + 1];
            cubeptr[x * 8 + 2] = calcCube[x * 3 + 2];
            cubeptr[x * 8 + 3] = calcColor[x * 3];
            cubeptr[x * 8 + 4] = calcColor[x * 3 + 1];
            cubeptr[x * 8 + 5] = calcColor[x * 3 + 2];
            cubeptr[x * 8 + 6] = calcTex[x * 2];
            cubeptr[x * 8 + 7] = calcTex[x * 2 + 1];
        }
        else if ((!colored) && (!textured) && (normaled))
        {
            cubeptr[x * 6] = calcCube[x * 3];
            cubeptr[x * 6 + 1] = calcCube[x * 3 + 1];
            cubeptr[x * 6 + 2] = calcCube[x * 3 + 2];
            cubeptr[x * 6 + 3] = calcNorm[x * 3];
            cubeptr[x * 6 + 4] = calcNorm[x * 3 + 1];
            cubeptr[x * 6 + 5] = calcNorm[x * 3 + 2];
        }
        else if ((!normaled) && (!textured) && (colored))
        {
            cubeptr[x * 6] = calcCube[x * 3];
            cubeptr[x * 6 + 1] = calcCube[x * 3 + 1];
            cubeptr[x * 6 + 2] = calcCube[x * 3 + 2];
            cubeptr[x * 6 + 3] = calcColor[x * 3];
            cubeptr[x * 6 + 4] = calcColor[x * 3 + 1];
            cubeptr[x * 6 + 5] = calcColor[x * 3 + 2];
        }
        else if ((!normaled) && (!colored) && (textured))
        {
            cubeptr[x * 5] = calcCube[x * 3];
            cubeptr[x * 5 + 1] = calcCube[x * 3 + 1];
            cubeptr[x * 5 + 2] = calcCube[x * 3 + 2];
            cubeptr[x * 5 + 3] = calcTex[x * 2];
            cubeptr[x * 5 + 4] = calcTex[x * 2 + 1];
        }
        else
        {
            cubeptr[x * 3] = calcCube[x * 3];
            cubeptr[x * 3 + 1] = calcCube[x * 3 + 1];
            cubeptr[x * 3 + 2] = calcCube[x * 3 + 2];
        }
    }
    return cubeptr;
}

float *AsterObject::getCube()
{
    return cubeVerts;
}

float *AsterObject::getColor()
{
    return cubeColors;
}

float *AsterObject::getNormals()
{
    return cubeNormals;
}

float *AsterObject::getTexCoords()
{
    return cubeTex;
}

unsigned int *AsterObject::getIndices()
{
    return cubeIndices;
}

void AsterObject::genMatrices()
{
    //! We generate three matrices:  the vertices of the cube,
    //! the normals and the texture coordinates.
    vec3 triangle[3];
    vec3 normal[3];
    vec3 color[3];
    vec2 texture[3];
    int countTex = 0, countNorm = 0;
    for (int x = 0; x < NUM_VERTICES; x += 3)
    {
        for (int y = 0; y < 3; y++)
        {
            triangle[y] = vec3(0.0f);
            normal[y] = vec3(0.0f);
            color[y] = vec3(0.0f);
            texture[y] = vec2(0.0f);
        }
        for (int y = 0; y < 3; y++)
        {
            triangle[0][y] = cube[indices[x]][y];
            triangle[1][y] = cube[indices[x + 1]][y];
            triangle[2][y] = cube[indices[x + 2]][y];
            color[0][y] = colors[indices[x]][y];
            color[1][y] = colors[indices[x + 1]][y];
            color[2][y] = colors[indices[x + 2]][y];
        }

        if (((triangle[0].x == triangle[1].x)
        && (triangle[0].x == triangle[2].x)
        && (triangle[1].x == triangle[2].x)) 
        && (triangle[0].x > 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[0];
                texture[y] = genTexture(normal[y], triangle[y]);
            }   
        }
        else if (((triangle[0].x == triangle[1].x)
        && (triangle[0].x == triangle[2].x)
        && (triangle[1].x == triangle[2].x)) 
        && (triangle[0].x < 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[3];
                texture[y] = genTexture(normal[y], triangle[y]);
            }
        }
        else if (((triangle[0].y == triangle[1].y)
        && (triangle[0].y == triangle[2].y)
        && (triangle[1].y == triangle[2].y)) 
        && (triangle[0].y > 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[1];
                texture[y] = genTexture(normal[y], triangle[y]);
            }
        }
        else if (((triangle[0].y == triangle[1].y)
        && (triangle[0].y == triangle[2].y)
        && (triangle[1].y == triangle[2].y)) 
        && (triangle[0].y < 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[4];
                texture[y] = genTexture(normal[y], triangle[y]);
            }
        }
        else if (((triangle[0].z == triangle[1].z)
        && (triangle[0].z == triangle[2].z)
        && (triangle[1].z == triangle[2].z)) 
        && (triangle[0].z > 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[2];
                texture[y] = genTexture(normal[y], triangle[y]);
            }
        }
        else if (((triangle[0].z == triangle[1].z)
        && (triangle[0].z == triangle[2].z)
        && (triangle[1].z == triangle[2].z)) 
        && (triangle[0].z < 0.0f))
        {
            for (int y = 0; y < 3; y++)
            {
                normal[y] = normals[5];
                texture[y] = genTexture(normal[y], triangle[y]);
            }
        }
/*
        cout << "\n\n\t\tTriangle Vertices\n\n\t";
        for (int z = 0; z < 3; z++)
        {
            cout << "\n\tTriangle " << z << " : " << triangle[z].x
            << ", " << triangle[z].y << ", " << triangle[z].z;
        }
*/
        int count = 0;
        for (int z = 0; z < 3; z++)
        {
            //!cout << "\n\tTriangle x,y,z ";
            //!cout << "\n\tNormal x,y,z  + Echo x,y,z ";
            for (int y = 0; y < 3; y++)
            {
                calcNorm[countNorm] = normal[z][y];
                calcCube[countNorm] = triangle[z][y];
                calcColor[countNorm] = color[z][y];
                //!cout << triangle[z][y] << ", ";
                //!cout << normal[z][y] << ", " << calcNorm[countNorm] 
                //!<< " : " << countNorm << " : ";
                countNorm++;
            }
            for (int y = 0; y < 2; y++)
            {
                calcTex[countTex] = texture[z][y];
                countTex++;
            }
        }
        //!cout << "\n\n";
    }
}
vec2 AsterObject::genTexture(vec3 normal, vec3 triangle)
{
    //!Texture coordinates are generated seperately.
    vec2 texture;
    if (normal.x != 0.0f)
    {
        if (triangle.y > 0.0f)
        {   
            texture.x = 1.0f;
        }
        else
        {
            texture.x = 0.0f;
        }
        if (triangle.z > 0.0f)
        {   
            texture.y = 1.0f;
        }
        else
        {
            texture.y = 0.0f;
        }
    }
    else if (normal.y != 0.0f)
    {
    
        if (triangle.x > 0.0f)
        {   
            texture.x = 1.0f;
        }
        else
        {
            texture.x = 0.0f;
        }
        if (triangle.z > 0.0f)
        {   
            texture.y = 1.0f;
        }
        else
        {
            texture.y = 0.0f;
        }
    }
    else if (normal.z != 0.0f)
    {
        if (triangle.x > 0.0f)
        {   
            texture.x = 1.0f;
        }
        else
        {
            texture.x = 0.0f;
        }
        if (triangle.y > 0.0f)
        {   
            texture.y = 1.0f;
        }
        else
        {
            texture.y = 0.0f;
        }
    }
    return texture;
}

float *AsterObject::genAsteroid()
{
    cubeptr = new float[smalltotal * 3];
    for (int x = 0; x < smalltotal; x++)
    {
        cubeptr[(x * 3)] = smallasteroid[smallindices[x]][0];
        cubeptr[(x * 3) + 1] = smallasteroid[smallindices[x]][1];
        cubeptr[(x * 3) + 2] = smallasteroid[smallindices[x]][2];
        cout << "\n\tGenAsteoid value:  " << x << " : " 
        <<cubeptr[(x * 3)] << ", " 
        << cubeptr[(x * 3) + 1] << ", " 
        << cubeptr[(x * 3) + 2];
    }
    return cubeptr;
}

int AsterObject::asteroidSize()
{
    return smalltotal;
}
