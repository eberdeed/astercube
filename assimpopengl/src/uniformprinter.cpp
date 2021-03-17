/**********************************************************
 *   UniformPrinter:  A class to encapsulate printing out
 *   the unforms from a shader program.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   12/2019 San Diego, California USA
 * ********************************************************/

#include "../include/uniformprinter.h"

UniformPrinter::UniformPrinter(int progObj)
{
    cout << "\n\n\tCreating UniformPrinter.\n\n";
    this->progObj = progObj;
    printUniforms();
}

UniformPrinter::~UniformPrinter()
{
    cout << "\n\n\tDestroying UniformPrinter.\n\n";
}

void UniformPrinter::printUniforms()
{
    int size, location;
    GLenum type;
    glGetProgramiv(progObj, GL_ACTIVE_UNIFORMS, &numUniforms);
    glGetProgramiv(progObj, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);
    uniformName = new char[maxUniformLen];
    cout << "\n\n\t\tActive Uniforms : Quantity:  " << numUniforms << ".\n\n";
    for (index = 0; index < numUniforms; index++)
    {
        glGetActiveUniform(progObj, index, maxUniformLen, NULL, &size, &type, uniformName);
        location = glGetUniformLocation(progObj, uniformName);
        cout << "\n\tUniform Index:  " << index << " location:  " << location;
        switch(type)
        {
            case GL_FLOAT:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT\n\n"; 
                break;
            case GL_FLOAT_VEC2:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_VEC2\n\n"; 
                break;
            case GL_FLOAT_VEC3:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_VEC3\n\n"; 
                break;
            case GL_FLOAT_VEC4:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_VEC4\n\n"; 
                break;
            case GL_INT:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_INT\n\n"; 
                break;
           case GL_INT_VEC2:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_INT_VEC2\n\n"; 
                break;
           case GL_INT_VEC3:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_INT_VEC3\n\n"; 
                break;
           case GL_INT_VEC4:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_INT_VEC4\n\n"; 
                break;
           case GL_UNSIGNED_INT:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_UNSIGNED_INT\n\n"; 
                break;
           case GL_UNSIGNED_INT_VEC2:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_UNSIGNED_INT_VEC2\n\n"; 
                break;
           case GL_UNSIGNED_INT_VEC3:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_UNSIGNED_INT_VEC3\n\n"; 
                break;
           case GL_UNSIGNED_INT_VEC4:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_UNSIGNED_INT_VEC4\n\n"; 
                break;
           case GL_BOOL:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_BOOL\n\n"; 
                break;
           case GL_BOOL_VEC2:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_BOOL_VEC2\n\n"; 
                break;
           case GL_BOOL_VEC3:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_BOOL_VEC3\n\n"; 
                break;
           case GL_BOOL_VEC4:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_BOOL_VEC4\n\n"; 
                break;
           case GL_FLOAT_MAT2:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_MAT2\n\n"; 
                break;
           case GL_FLOAT_MAT3:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_MAT3\n\n"; 
                break;
           case GL_FLOAT_MAT4:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_MAT4\n\n"; 
                break;
           case GL_FLOAT_MAT2x3:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_MAT2x3\n\n"; 
                break;
           case GL_FLOAT_MAT2x4:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_MAT2x4\n\n"; 
                break;
           case GL_FLOAT_MAT3x2:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_MAT3x2\n\n"; 
                break;
           case GL_FLOAT_MAT3x4:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_MAT3x4\n\n"; 
                break;
           case GL_FLOAT_MAT4x2:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_MAT4x2\n\n"; 
                break;
           case GL_FLOAT_MAT4x3:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_FLOAT_MAT4x3\n\n"; 
                break;
           case GL_SAMPLER_2D:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_SAMPLER_2D\n\n"; 
                break;
           case GL_SAMPLER_3D:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_SAMPLER_3D\n\n"; 
                break;
           case GL_SAMPLER_CUBE:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_SAMPLER_CUBE\n\n"; 
                break;
           case GL_SAMPLER_2D_SHADOW:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_SAMPLER_2D_SHADOW\n\n"; 
                break;
           case GL_SAMPLER_2D_ARRAY:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_SAMPLER_2D_ARRAY\n\n"; 
                break;
           case GL_SAMPLER_2D_ARRAY_SHADOW:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_SAMPLER_2D_ARRAY_SHADOW\n\n"; 
                break;
           case GL_SAMPLER_CUBE_SHADOW:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_SAMPLER_CUBE_SHADOW\n\n"; 
                break;
           case GL_INT_SAMPLER_2D:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_INT_SAMPLER_2D\n\n"; 
                break;
           case GL_INT_SAMPLER_3D:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_INT_SAMPLER_3D\n\n"; 
                break;
           case GL_INT_SAMPLER_CUBE:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_INT_SAMPLER_CUBE\n\n"; 
                break;
           case GL_INT_SAMPLER_2D_ARRAY:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_INT_SAMPLER_2D_ARRAY\n\n"; 
                break;
           case GL_UNSIGNED_INT_SAMPLER_2D:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_UNSIGNED_INT_SAMPLER_2D\n\n"; 
                break;
           case GL_UNSIGNED_INT_SAMPLER_3D:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_UNSIGNED_INT_SAMPLER_3D\n\n"; 
                break;
           case GL_UNSIGNED_INT_SAMPLER_CUBE:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_UNSIGNED_INT_SAMPLER_CUBE\n\n"; 
                break;
          case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  GL_UNSIGNED_INT_SAMPLER_2D_ARRAY\n\n"; 
                break;
          default:
                cout << "\n\n\tName:  " << uniformName << "  Location:  " << location << "  Type:  Unknown\n\n"; 
                break;
 
        };
    };
}
