/*******************************************************************
 * Shader:  A class to encapsulate the creation and use of a set of
 * shaders.  
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * March 2020 San Diego, California USA
 * ****************************************************************/

#ifndef SHADER_H
#define SHADER_H

#include "commonheader.h"
#include <iostream>
#include <string>
// GLM The OpenGL math library
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

/** \class Shader A class to encapsulate the uploading, compiling, 
 * linking and use of a shader.  This class will create a shader 
 * binary and reload it.  The binary will be recreated if it ceases 
 * to validate as a shader program.
 */

class Shader
{
public:
    /** \brief The constructor signals the creation of the class.
     */
    Shader();
    /** \brief The destructor signals the destruction of the class,
     *  and deletes the shader program.
     */  
    ~Shader();
    /** \brief Read and build the shader program from two files,
     *  and provide the file name for the created binary.
     *  Delete the two shader objects afterwards.
     *  The variables are as follows:
     *  vertexPath : The vertex shader's location in the file structure.
     *  fragmentPath : The fragment shader's location in the file structure.
     *  outputFile : The name given to the output binary.
     */
    /* Functions */
    /** \brief A function to create the shader program from the
     * vertex and fragment shader code and store the result in 
     * a binary program in the  "~/.config" directory.
     * vertexPath:  The file location for the vertex shader code.
     * fragmentPath:  The file location for the fragment shader code.
     * outputFile: The name of the resulting binary to be
     * stored in the "~/.config" directory.
     */
    void initShader(string vertexPath, string fragmentPath, 
    string outputFile);
    /** \brief Create either the vertex or fragment shader from a file.
     */
    unsigned int createShader(unsigned int type, string fpath);
    /** \brief Use the program object for display.
     */
    void Use();
    /** \brief Create the shader program binary save it to a file,
     * and delete the binary pointer afterward.
     */
    bool createBinary();
    /** \breif Error reporting. 
     */
    string getError();
    /** \brief Utility uniform functions that set values in the shader program.
     */
    void setBool(const string name, bool value) const;  
    void setInt(const string name, int value) const;   
    void setFloat(const string name, float value) const;
    void setVec2(const std::string name, vec2 value) const;
    void setVec3(const std::string name, vec3 value) const;
    void setVec4(const string name, vec4 value) const; 
    void setMat4(const string name, mat4 value) const;    
    
    /** Variables **/
    //! The shader program object.
    GLuint Program;
protected:
    /** Interesting that the shader code has to be wrapped
     *  inside an array.
     */
    typedef char *chararray[1];
    chararray glShaderCode;
    //! Indicates success of OpenGL function calls.
    int success = 0;
    //! Length of an OpenGL error message.
    int infoLength = 0;
    //! Handles for vertex and fragment shaders respectively.
    GLuint vertex, fragment;
    //! The length of the program in bytes.
    int progLength = 0;
    //! The length of the program binary in bytes.
    int progLenRet = 0;
    //! Formats for the stored binary program.
    GLenum *valFormats;
    //! The value of an individual format type.
    GLenum format = 0;
    //! A name for the shader.
    string shadername;
    //! A variable indicating the successful loading of a stored program binary.
    int response = 0;
    //! The binary program pointer.
    unsigned char *binary;
    /** The output file name.  The final stored binary is stored
     * int the .config directory of the user's home directory (~/.config).
     */
    string outputFile;
    //! Debugging information.
    bool debug1 = false;
};
  
#endif //SHADER_H
