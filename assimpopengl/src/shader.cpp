/*******************************************************************
 * Shader:  A class to encapsulate the creation and use of a set of
 * shaders. Note this class requires a seperate "shaders" directory
 *  to store the shaders in.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * April 2020 San Diego, California USA
 * ****************************************************************/

#include "../include/shader.h"


Shader::Shader()
{
    cout << "\n\n\tCreating Shader.\n\n";
}

Shader::~Shader()
{
    cout << "\n\n\tDestroying Shader:  " << shadername << ".\n\n";
    glDeleteProgram(Program);
}

void Shader::initShader(string vertexPath, string fragmentPath, 
    string outputFile)
{
    cout << "\n\n\tIn initShader.\n\n";
    string home = getenv("HOME");
    this->shadername = outputFile;
    outputFile =  home + "/.config/" + outputFile;
    this->outputFile = outputFile;
    if (!exists(vertexPath))
    {
        cout << "\n\n\tError no vertex shader at " << vertexPath << ".\n\n";
        exit(-1);
    }
    if (!exists(fragmentPath))
    {
        cout << "\n\n\tError no fragment shader at " << vertexPath << ".\n\n";
        exit(-1);
    }
    char *infoLog;
    int numFormats = 0;
    GLenum *valFormats;
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &numFormats);
    cout << "\n\n\tNumber of shader formats:  " << numFormats << "\n\n";
    Program = glCreateProgram();
    if (numFormats == 0)
    {
        response = false;
    }
    else
    {
        if (numFormats > 1)
        {
            valFormats = new GLenum[numFormats];
            glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, (int*)valFormats);
            format = valFormats[0];
            for (int x = 0; x < numFormats; x++)
            {
                cout << "\n\nFormat " << x << " : " << valFormats[x] << " : " << format;
            }
            cout << "\n\n";
        }
        else
        {
            valFormats = new GLenum(0);
            glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, (int*) valFormats);
            format = valFormats[0];
            cout << "\n\n\tFormat " << 0 << " : " << valFormats[0] << " : " << format;
        }
        if (!format)
        {
            cout << "\n\tNo format available for program binary:  " << outputFile << ".\n\n";
            response = false;
        }
        if (!exists(outputFile))
        {
            cout << "\n\tThe shader binary file:  " << outputFile << " does not exist.\n\n";
            response = false;
        }
        else
        {
            boost::filesystem::ifstream shaderFile(outputFile, ios_base::in | ios_base::binary);
            if (!shaderFile.is_open())
            {
                cout << "\n\n\tError opening file " << outputFile << ".\n\n";
                response = false;
                if (debug1)
                {            
                    exit(-1);
                }
            }
            else
            {
                GLsizei fileSize = file_size(path(outputFile.c_str()));
                if (debug1)
                {
                    cout << "\n\tFile Size:  " << fileSize;
                }
                char shaderBinary[fileSize];
                if (debug1)
                {
                    cout << "\n\tCreated binary array to hold shader.\n\n";
                }
                shaderFile.read(shaderBinary, fileSize);
                shaderFile.close();
                glProgramBinary(Program, format, (GLvoid*) shaderBinary, fileSize);
                glValidateProgram(Program);
                if (fileSize > 0)
                {
                    cout << "\n\n\tSuccessfully loaded and validated the pre-compiled agregate "
                    << "program binary:  " << outputFile <<  ".\n\tThe program has binary format " 
                    << format << " and size " << fileSize << " bytes.\n\n";
                }
                else
                {
                    response = false;
                }
            }
        }
    }
    cout << "\n\n\tCompleted program retrieval.\n\n";
    if (!response)
    {
        vertex = createShader(GL_VERTEX_SHADER, vertexPath);
        fragment = createShader(GL_FRAGMENT_SHADER, fragmentPath);
        if ((!vertex) || (!fragment))
        {
            cout << "\n\n\tError compiling shaders for shader:  " 
            << outputFile << "\n\n";
            exit(1);
        }
        try {
            glProgramParameteri(Program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
            glAttachShader(Program, vertex);
            glAttachShader(Program, fragment);
            glLinkProgram(Program);
            string errorstr = getError();
            cout << "\n\t" << errorstr;
        } catch(exception exc) {
            cout << "\n\n\tError creating shader program:  " << exc.what() << "\n\n";
            if (Program) {
               glDeleteProgram(Program);
            }
            exit(-1);
        }
        
        glGetProgramiv(Program, GL_LINK_STATUS, &response);
        // Print linking errors if any
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &infoLength);
        if(infoLength > 0)
        {
            infoLog = new char[infoLength];
            glGetProgramInfoLog(Program, infoLength, NULL, infoLog);
            cout << "\n\n\tShader Program Link Error\n\t" << infoLog 
            << "\n\tFor shader:  " << outputFile << "\n\n";
            exit(-1);
        }
        else if (response)
        {
            cout << "\n\n\tShader agregate program created "
            << "and linked.\n\n";
        }
        else
        {
            cout << "\n\n\tShader " << shadername << " failed to link.\n\n";
            exit(-1);
        }
        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(createBinary())
        {
            cout << "\n\n\tShader program binary " << outputFile 
            << " compiled and saved.\n\n";
        }
        else
        {
            cout << "\n\n\tShader program binary " << outputFile 
            << " failed to compile and save.\n\n";
        }
    }
}

unsigned int Shader::createShader(unsigned int type, string fpath)
{
    unsigned int shaderobj;
    int codeLen = 0;
    string errorstr;
    boost::filesystem::ifstream shaderFile(fpath);
    boost::filesystem::path filePath(fpath);
    codeLen = file_size(filePath);
    string shaderCode(codeLen, '\0');
    if (!shaderFile.is_open())
    {
        cout << "\n\n\tError opening file " << fpath << ".\n\n";
        return 0;
    }
    // Read file's buffer contents into streams
    if (codeLen > 0)
    { // standard C I/O file reading loop
        
        shaderFile.read(&shaderCode[0], codeLen);
    }
    else
    {
        cout << "\n\n\tNo data for shader: " << fpath << "\n\n";
        return 0;
    }
    shaderFile.close();

    // Vertex Shader
    try
    {
        shaderobj = glCreateShader(type);
        errorstr = getError();
        glGetShaderiv(shaderobj, GL_INFO_LOG_LENGTH, &infoLength);
        if (!shaderobj)
        {
            cout << "\n\n\tError creating shader object:  " << fpath << ".\n\n";
            if (errorstr != "Unknown value.")
            {
                cout << "\n\n\tCreating the shader object produced:  " << errorstr << "\n\n";
            }
            exit(-1);
        }
        else if(infoLength > 0)
        {
            char infoLog[infoLength];
            glGetShaderInfoLog(shaderobj, infoLength, NULL, infoLog);
            cout << "\n\nShader Program Creation Error\n" << infoLog << "\n\tFor shader:  " << fpath << ".\n\n";
            exit(-1);
        }
        else
        {
            cout << "\n\n\tCreated the shader object:  " << fpath << ".\n\n";
        }
        try 
        {
            glShaderCode[0] = (char*) shaderCode.c_str();
            if (codeLen != strlen(glShaderCode[0]))
            {
                cout << "\n\n\tThe initial length of the shader string is " << codeLen
                << " and the current length is " << strlen(glShaderCode[0]) 
                << " for shader " << fpath << "\n\n";
                exit(-1);
            }
            glShaderSource(shaderobj, 1, glShaderCode, &codeLen);
            errorstr = getError();
            if (errorstr != "GL_NO_ERROR")
            {
                cout << "\n\n\tAdding source code to the shader object produced:  " << errorstr << "\n\n";
                exit(-1);
            }
            glCompileShader(shaderobj);
            // Print compile errors if any
            glGetShaderiv(shaderobj, GL_INFO_LOG_LENGTH, &infoLength);
        } 
        catch(exception exc) 
        {
            cout << "\n\n\tError creating shader object from code:  " << exc.what() << "\n\n";
            exit(-1);
        }
        if ((!shaderobj) && (infoLength == 0))
        {
            cout << "\n\n\tError compiling shader object for shader:  " << fpath << ".\n\n";
            if (debug1)
            {            
                exit(-1);
            }
        }
        else if(infoLength > 0)
        {
            char infoLog[infoLength];
            glGetShaderInfoLog(shaderobj, infoLength, NULL, infoLog);
            cout << "\n\n\tShader compilation error: \n" << infoLog 
            << "\n\tFor shader " << fpath << "\n\n";
            glDeleteShader(shaderobj);
            return 0;
        }
        else
        {
             cout << "\n\n\tShader compiled.\n\n";
        }
        return shaderobj;
    }
    catch(exception exc)
    {
        cout << "\n\n\tError making shader:  " << exc.what() << "\n\n";
        return 0;
    }
}
string Shader::getError()
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

void Shader::Use() 
{ 
    if (debug1)
    {
        cout << "\n\tShader " << shadername << " is being used.";
    }
    glUseProgram(Program); 
    string errorstr = getError();
    cout << "\n\t" << errorstr;
}   

bool Shader::createBinary()
{
    if (progLength <= 0)
    {
        cout << "\n\n\tShader program length less than one.\n\n";
        progLength = 1000000;
        if (debug1)
        {
            return false;
        }
    }
    binary = new unsigned char[progLength];
    glGetProgramBinary(Program, progLength, &progLenRet, &format, (GLvoid*) binary);
    if (progLength != progLenRet)
    {
        cout << "\n\n\tWarning program length of " << progLength 
        << " does not equal the size of the created binary "
        << progLenRet << " with format " << format
        << "\n\n";
        if (debug1)
        {
            return false;
        }
    }
    cout << "\n\n\tSaving shader binary at:  " << outputFile << ".\n\n";
    boost::filesystem::ofstream shaderFile(outputFile, ios_base::out | ios_base::binary);
    if (!shaderFile.is_open())
    {
        cout << "\n\n\tError opening file " << outputFile << ".\n\n";
        return false;
    }
    for (int x = 0; x < progLenRet; x++)
    { // Standard C I/O file writing loop.
       shaderFile.put(binary[x]);
    }
    //shaderFile.close();
    cout << "\n\n\tShader agregate binary program created "
    << "and saved.  The program has length " << progLenRet 
    << " bytes.\n\tAnd is saved at:  " << outputFile << ".\n\n";
    free(binary);
    return true;
}
    
void Shader::setBool(const std::string name, bool value) const
{         
    glUniform1i(glGetUniformLocation(Program, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string name, int value) const
{ 
    glUniform1i(glGetUniformLocation(Program, name.c_str()), value); 
}
void Shader::setFloat(const std::string name, float value) const
{ 
    glUniform1f(glGetUniformLocation(Program, name.c_str()), value); 
} 
void Shader::setVec2(const std::string name, vec2 value) const
{ 
    glUniform2fv(glGetUniformLocation(Program, name.c_str()), 1, value_ptr(value)); 
} 
void Shader::setVec3(const std::string name, vec3 value) const
{ 
    glUniform3fv(glGetUniformLocation(Program, name.c_str()), 1, value_ptr(value)); 
} 
void Shader::setVec4(const std::string name, vec4 value) const
{ 
    glUniform4fv(glGetUniformLocation(Program, name.c_str()), 1, value_ptr(value)); 
} 
void Shader::setMat4(const std::string name, mat4 value) const
{ 
    glUniformMatrix4fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, &value[0][0]); 
} 
