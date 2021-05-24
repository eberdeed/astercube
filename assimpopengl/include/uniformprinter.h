/**********************************************************
 *   UniformPrinter:  A class to encapsulate printing out
 *   the unforms from a shader program.  Gives the location
 *   of the uniform and it's status.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   12/2019 San Diego, California USA
 * ********************************************************/

#ifndef UNIFORMPRINTER_H
#define UNIFORMPRINTER_H
#include "commonheader.h"

/** \class UniformPrinter A class to encapsulate printing out the unforms from a 
 * shader program.  Gives the location of the uniform and 
 * it's status.
 */
class UniformPrinter
{
public:
    //! \brief Constructor:  requires the shader program object.
    UniformPrinter(int progObj);
    //! \brief Destructor.
    ~UniformPrinter();
protected:
    //! Print the active uniforms.
    void printUniforms();
    //! Class global variables.
    int maxUniformLen, numUniforms, index, progObj;
    char *uniformName;
    
};



#endif // UNIFORMPRINTER_H
