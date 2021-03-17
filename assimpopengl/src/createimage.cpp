/**********************************************************
 *   CreateImage:  A class to encapsulate loading an image
 *   into a GLvoid blob.  Designed for OpenGL. This class
 *   assumes that there is an images directory with the 
 *   graphics in it.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   April 2020 San Diego, California USA
 * ********************************************************/

#include "../include/createimage.h"

CreateImage::CreateImage()
{
    cout << "\n\n\tCreating CreateImage.\n\n";
}

CreateImage::~CreateImage()
{
    cout << "\n\n\tDestroying CreateImage.\n\n";
}

bool CreateImage::setImage(string imagefile)
{
    cout << "\n\n\tIn setImage().\n\n";
    //imagefile = "../images/" + imagefile;
    try
    {
        //! Free Image Plus Image loads standard picture.
        if (!txtImage.load(imagefile.c_str()))
        {
            cout << "\n\n\tImage file " << imagefile << " failed to load in createimage.\n";
            return false;
        }
        else if (debug1)
        {
            cout << "\n\n\tImage file " << imagefile << " successfully loaded.\n\n";
        }
    }
    catch (exception exc)
    {
        cout << "\n\n\tError loading file " << imagefile << " : " << exc.what() << "\n\n";
        return false;
    }
    size = 0;
    int counter = 0;
    //! Convert image to four 8 bit fields RGBA.
    txtImage.convertTo32Bits();
    width = (GLsizei) txtImage.getWidth();
    height = (GLsizei) txtImage.getHeight();
    size = width * height * 4;
    line = width * 4;
    ivec4 test;
    // Load the image into an unsigned char array.
    pixels = new unsigned char[size];
    for (unsigned int y = 0; y < height; y++)
    {
        picLine = txtImage.getScanLine(y);
        for (unsigned int x = 0; x < line; x += 4)
        {
            if (counter > size)
            {
                continue;
            }
            test.x = counter;
            pixels[counter++] = (unsigned char) picLine[x + 2];
            test.y = counter;
            pixels[counter++] = (unsigned char) picLine[x + 1];
            test.z = counter;
            pixels[counter++] = (unsigned char) picLine[x];
            test.w = counter;
            pixels[counter++] = (unsigned char) picLine[x + 3];
            if (int(pixels[test.w]) == 0)
            {
                pixels[test.x] = pixels[test.y] = pixels[test.z] = 0;
            }
            
                
        }
    }
    txtImage.clear();
    return true;
}

//! Accessor functions to pass along the data.
GLsizei CreateImage::getWidth()
{
    return width;
}

GLsizei CreateImage::getHeight()
{
    return height;
}

GLsizei CreateImage::getSize()
{
    return size;
}

GLvoid *CreateImage::getData()
{
    return (GLvoid*) pixels;
}

//! Use the CreateImage class to turn an image into a texture.
GLuint CreateImage::textureObject()
{
    //Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    GLsizei width, height;
    width = getWidth();
    height = getHeight();
    GLvoid *image = getData();
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);    
    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

void CreateImage::createSkyBoxTex(GLuint &textureID, string filenames[6])
{
    // Loads a cubemap texture from 6 individual texture faces
    // Order should be:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front) 
    // -Z (back)
    GLsizei width, height;
    void *pixel_data = nullptr;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    //! Six images, one texture ID.
    for (int i = 0; i < 6; i++)
    {
        //! The order of images in a skybox is reversed.\n\n';
        if (setImage(filenames[5 - i]))
        {
            width = getWidth();
            height = getHeight();
            pixel_data = getData();
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
        }
        else
        {
            cout << "\n\n\tImage load failure.  "
            << "Only a partial load is present.\n\n";
            exit(-1);
        }
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    return;
}
void CreateImage::create2DTexArray(GLuint &textureID,  vector<string>filenames)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
    int count = 0;
    if (setImage(filenames[0]))
    {
        unsigned char *pixel_data = new unsigned char[size * filenames.size()];
        for (int x = 0 * size; x < size; x++)
        {
            pixel_data[count++] = pixels[x];
        }
        for (int i = 1; i < filenames.size(); i++)
        {
            setImage(filenames[i]);
            for (int x = 0; x < size; x++)
            {
                pixel_data[count++] = pixels[x];
            }
        }
        if (debug1)
        {
            cout << "\n\n\tPixels loaded:  " << count 
            << "  Pixels calculated:  " << filenames.size() * size << "\n\n";
        }
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, filenames.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) pixel_data);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }
    else
    {
        cout << "\n\n\tImage load failure.  "
        << "Only a partial load is present.\n\n";
    }
    return;
}
