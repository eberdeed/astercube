/**********************************************************
 *   AsterCube:  A class to display an arbitrary Blender
 *   objects in a skybox with a given number of objects in OpenGL.  
 *   The objects and their locations are passed into the 
 *   program by the Objects class function. It is possible
 *   to create a terrain. The terrain  and skybox are created 
 *   by the Terrain and SkyBox classes respectively.  The Objects 
 *   are displayed by the assimpopengl library. In this
 *   case we are instancing 6 asteroids 36 times each.
 *   Blender can be found at www.blender.org and similarly
 *   OpenGL can be found at www.khronos.org.  To understand
 *   the inner workings of OpenGL I used the book:
 *   "OpenGL ES 3.0 Programming Guide Second Edition" 
 *   by Dan Ginsburg and Budirijanto Purnomo published by 
 *   Addison-Wesley 2014. Also useful was:  www.learnopengl.com.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   February 2021 San Diego, California USA
 *   Copyright Maureen A. Quinn 03/2021
 * ********************************************************/

#ifndef ASTERCUBE_H
#define ASTERCUBE_H

#include "commonheader.h"
#include "../assimpopengl/include/camera.h"
#include <assimp/camera.h>
#include "skybox.h"
#include "terrain.h"
#include "objects.h"

/**   \class AsterCube
 *   A class to display arbitrary blender objects in a sky box with
 *   an assigned terrain in OpenGL. SDL2 is used to provide windowing 
 *   support.
 */
class AsterCube
{
public:
    /** \brief Constructor to instantiate the Object, Terrain and SkyBox
     *  classes.
     */
    AsterCube();
    /** \brief Destructor to delete the instatiated classes.
     */
    ~AsterCube();
    /** \brief the Execution loop.
     */
    void exec();
private:
    /** \brief Window and viewport resizing.
     */
    void framebufferSize(int width, int height);
    /** \brief Capture a key down event.
     */
    void keyDown(SDL_Event e);
    /** \brief Capture a window resize or close event.
     */
	void windowEvent(SDL_Event e);
    /** \brief Mouse movement processing.
     */
    void mouseMove(SDL_Event e);
    /** \brief Display error messages.
     */
    void logSDLError(ostream &os, const string &msg);

    //! Settings
    const unsigned int SCR_WIDTH = 1000;
    const unsigned int SCR_HEIGHT = 900;
    bool quit, firstMouse = true;
    const float onedegree = acos(-1) / 180.0f;
    float xpos, ypos, lastX, lastY;
    mat4 view, projection, model = mat4(1.0f);
    vec3 viewPos;
    float size = 1000.0f;
    float limit = 80.0f;
    /** Camera class to manage camera position and 
     *  orientation.
     */
    Camera *camera;
    //! The sky box.
    SkyBox *skybox;
    //! The terrain.
    Terrain *terrain;
    //! The blender objects.
    Objects *objects;
    // SDL window variables.
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_GLContext context;
    //! Timing for the animation and the camera.
    chrono::system_clock::time_point intbegin, intend;
    //! Flags for program flow.
    bool test = false;
    bool altSet = false;
    bool debug1 = false;
    //! The sky box sampler cube.
    unsigned int skyboxTex;
    int amount;
    //! The Xlib objects to determine display size.
    _XDisplay* d;
    Screen*  s;
};

int main(int argc, char **argv)
{
    AsterCube outerspace;
    return 0;
}

#endif // ASTERCUBE_H
