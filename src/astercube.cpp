/**********************************************************
 *   AsterCube:  A class to display an arbitrary Blender
 *   object in OpenGL.  The objects and their locations are 
 *   passed into the program by the setupObjects function.
 *   Blender can be found at www.blender.org and similarly
 *   OpenGL can be found at www.khronos.org.  To understand
 *   the inner workings of OpenGL I used the book:
 *   "OpenGL ES 3.0 Programming Guide Second Edition" 
 *   by Dan Ginsburg and Budirijanto Purnomo published by 
 *   Addison-Wesley 2014. Also useful was:  www.learnopengl.com.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   01/2020 San Diego, California USA
 *   Copyright Maureen A. Quinn 02/2020
 * ********************************************************/


#include "../include/astercube.h"


AsterCube::AsterCube()
{
    /** I pass creation and destruction messages
     *  from each class to ensure the class 
     *  is properly handled.
     */
    cout << "\n\n\tCreating AsterCube\n\n";
    quit = false;
    //! \brief amount The total number of asteroids
    //! n^3 = amount should have n as an integer.
    amount = 216;
    d = XOpenDisplay(NULL);
    s = DefaultScreenOfDisplay(d);
    exec();
}

   /** Deleting the objects produces destroy messages
     *  from the object deleted.
     */
AsterCube::~AsterCube()
{
    cout << "\n\n\tDestroying AsterCube\n\n";
    delete skybox;
    //delete terrain;
    delete objects;
    delete camera;
    cout << "\n\n\tDeleted Camera.\n\n";
}


void AsterCube::exec()
{
    cout << "\n\n\tIn exec.\n\n";
    SDL_Event e;
    quit = false;
    try
    {
        // Setup the window
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            logSDLError(cout, "SDL_Init");
            exit(1);
        }
        else
        {
            cout << "\n\n\tInitialized SDL.\n\n";
        }
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

        window = SDL_CreateWindow("OpenGL Demo", 500, 200, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
        if (window == nullptr){
            logSDLError(cout, "CreateWindow");
            SDL_Quit();
            exit(1);
        }
        else
        {
            cout << "\n\n\tCreated SDL window.\n\n";
        }
        
        context = SDL_GL_CreateContext(window);
        if (context == nullptr){
            logSDLError(cout, "CreateContext");
            SDL_DestroyWindow(window);
            SDL_GL_DeleteContext(context);
            SDL_Quit();
            exit(1);
        }
        else
        {
            cout << "\n\n\tCreated SDL context.\n\n";
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer == nullptr){
            logSDLError(cout, "CreateRenderer");
            SDL_DestroyWindow(window);
            SDL_GL_DeleteContext(context);
            SDL_Quit();
            exit(1);
        }
        else
        {
            cout << "\n\n\tCreated renderer.\n";
        }
        SDL_ShowWindow(window);
        SDL_GL_MakeCurrent(window, context);
        SDL_GL_SetSwapInterval(1);
        //! We do this here to initialize GLEW.
        glewExperimental=true;
        GLenum err=glewInit();
        if(err!=GLEW_OK)
        {
            //Problem: glewInit failed, something is seriously wrong.
            cout<<"\n\n\tThe function glewInit failed, aborting."<<endl;
            SDL_DestroyWindow(window);
            SDL_GL_DeleteContext(context);
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            exit(1);
        }
        else
        {
            cout << "\n\n\tInitialized glew.\n";
        }
        cout << "\n\n\tUsing GLEW Version: " << glewGetString(GLEW_VERSION) << "\n\n";
        glEnable(GL_DEPTH_TEST);
        bool enabled = glIsEnabled(GL_DEPTH_TEST);
        if (enabled)
        {
            cout << "\n\n\tDepth Test Enabled\n\n";
        }
        else
        {
            cout << "\n\n\tDepth Test Not Enabled\n\n";
        }
        glDepthFunc(GL_LESS);
        //glFrontFace(GL_CCW);
        glDisable(GL_CULL_FACE);
        enabled = glIsEnabled(GL_CULL_FACE);
        if (enabled)
        {
            cout << "\n\n\tCull Face Enabled\n\n";
        }
        else
        {
            cout << "\n\n\tCull Face Not Enabled\n\n";
        }
        //glCullFace(GL_BACK);
        glDepthRange(0.1f, 1000.0f);
        skybox = new SkyBox();
        //terrain = new Terrain();
        objects = new Objects();
        skybox->setScale(size);
        //terrain->setScale(size);
        objects->setScale(limit);
        objects->initObjects();
        camera = new Camera(SCR_WIDTH, SCR_HEIGHT, vec3(0.0f, -7.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f));
        skybox->initSkyBox();
        skyboxTex = skybox->getSkyBox();
        objects->setSkyBox(skyboxTex);
    }
    catch(exception exc)
    {
        cout << "\n\n\tProgram Initialization Error:  " << exc.what() << "\n\n";
    }
    //! render loop
    //! -----------
    while (!quit)
    {
        //! Grab a time to adjust camera speed.
        intbegin  = chrono::system_clock::now();
        //! Find the camera.
        //! render
        // ------
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        view = camera->getViewMatrix(); //! render
        viewPos = camera->getPosition();
        projection = camera->getPerspective();
        skybox->drawSkyBox(model, view, projection);
        //terrain->drawTerrain(model, view, projection);
        objects->drawObjects(model, view, projection, viewPos);
        intend = chrono::system_clock::now();
        while (SDL_PollEvent(&e))
        {
            keyDown(e);
            windowEvent(e);
            mouseMove(e);
        };
        SDL_GL_SwapWindow(window);
        if (debug1)
        {
            cout << "\n\n\tThe window is replaced with a new window.\n\n";
        }
    }

    cout << "\n\n\tWindow " << e.window.windowID << " closed.\n\n";
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(context);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return;
}
// ---------------------------------------------------------------------------------------------------------
void AsterCube::keyDown(SDL_Event e)
{
    int delta = (int)
    //! Use timing to create a cameraSpeed variable.
    chrono::duration_cast<chrono::milliseconds>(intend -
    intbegin).count();
    float cameraSpeed = 0.5f * delta;
    //! Motion keys.
    if (e.type == SDL_KEYDOWN) 
    {
        if (debug1)
        {
            cout << "\n\n\tIn keyDown:  " << SDL_GetKeyName(e.key.keysym.sym) << "\n\n";
            cout << "\n\tdelta:  " << delta << " cameraSpeed:  " << cameraSpeed << "\n";
        }
        switch (e.key.keysym.sym)
        {
            //! Forward motion.
            case SDLK_w:
                camera->processKeyboard(Camera::Camera_Movement::FORWARD, cameraSpeed);
                break;
            //! Backwards motion.
            case SDLK_s:
                camera->processKeyboard(Camera::Camera_Movement::BACKWARD, cameraSpeed);
                break;
            //! Move left.
            case SDLK_a:
                camera->processKeyboard(Camera::Camera_Movement::LEFT, cameraSpeed);
                break;
            //! Move right.
            case SDLK_d:
                camera->processKeyboard(Camera::Camera_Movement::RIGHT, cameraSpeed);
                break;
            //! Move up.
            case SDLK_r:
                camera->processKeyboard(Camera::Camera_Movement::UP, cameraSpeed);
                break;
            //! Move down.
            case SDLK_f:
                camera->processKeyboard(Camera::Camera_Movement::DOWN, cameraSpeed);
                break;
            //! Reset the camera.
            case SDLK_z:
                camera->resetCamera();
                break;
            //! Reverse the camera.
            case SDLK_x:
                camera->reverseDirection();
                break;
            //! Zoom keys.
            //! Zoom in.
            case SDLK_UP:
                camera->processMouseScroll(Camera::Camera_Movement::CLOSER);
                break;
            //! Zoom out.
            case SDLK_DOWN:
                camera->processMouseScroll(Camera::Camera_Movement::AWAY);
                break;
            case SDLK_ESCAPE:
                cout << "\n\n\tIn SDL Escape.\n\n";
                quit = true;
                break;
           case SDLK_RALT:
                altSet = true;
                break;
            case SDLK_LALT:
                altSet = true;
                break;
            case SDLK_RETURN:
                if (altSet)
                {
                    cout << "\n\n\tSet window fullscreen.\n\n";
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    framebufferSize(s->width, s->height);
                    cout << "\n\t Window Dimensions:  " << s->width << ", " << s->height << "\n";
                    altSet = false;
                }
                break;
        }
    }
}    

void AsterCube::mouseMove(SDL_Event e)
{
    if (e.type == SDL_MOUSEWHEEL)
    {
            if(e.wheel.y > 0)
            {
                camera->processMouseScroll(Camera::Camera_Movement::CLOSER);
            }
            else if (e.wheel.y < 0)
            {
                camera->processMouseScroll(Camera::Camera_Movement::AWAY);
            }
    }
    else if( e.type == SDL_MOUSEMOTION)
    {
        camera->processMouseMovement(e.motion.xrel, e.motion.yrel);
    }
    
}    

void AsterCube::windowEvent(SDL_Event e)
{
    if (e.type == SDL_WINDOWEVENT) {
        switch (e.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            cout << "\n\n\tWindow " << e.window.windowID << " shown.\n\n";
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            cout << "\n\n\tWindow " << e.window.windowID << " hidden.\n\n";
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            cout << "\n\n\tWindow " << e.window.windowID << " exposed.\n\n";
            break;
        case SDL_WINDOWEVENT_MOVED:
            cout << "\n\n\tWindow " << e.window.windowID << " moved to " 
            << e.window.data1 << ", " << e.window.data2 << "\n\n";
            break;
        case SDL_WINDOWEVENT_RESIZED:
            cout << "\n\n\tWindow " << e.window.windowID << " resized to " 
            << e.window.data1 << ", " << e.window.data2 << "\n\n";
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            cout << "\n\n\tWindow " << e.window.windowID << " size changed to " 
            << e.window.data1 << ", " << e.window.data2 << "\n\n";
            glViewport(0, 0, e.window.data1, e.window.data2);
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            cout << "\n\n\tWindow " << e.window.windowID << " minimized.\n\n";
            break;
            glViewport(0, 0, 0, 0);
        case SDL_WINDOWEVENT_MAXIMIZED:
            cout << "\n\n\tWindow " << e.window.windowID << " maximized.\n\n";
            glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
            break;
        case SDL_WINDOWEVENT_RESTORED:
            cout << "\n\n\tWindow " << e.window.windowID << " restored.\n\n";
            break;
        case SDL_WINDOWEVENT_ENTER:
            cout << "\n\n\tMouse entered window " << e.window.windowID << ".\n\n";
            break;
        case SDL_WINDOWEVENT_LEAVE:
            cout << "\n\n\tMouse left window " << e.window.windowID << ".\n\n";
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            cout << "\n\n\tWindow " << e.window.windowID << " gained keyboard focus.\n\n";
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            cout << "\n\n\tWindow " << e.window.windowID << " lost keyboard focus.\n\n";
            break;
        case SDL_WINDOWEVENT_CLOSE:
            cout << "\n\n\tWindow " << e.window.windowID << " closed.\n\n";
            quit = true;
            break;
        case SDL_WINDOWEVENT_TAKE_FOCUS:
            cout << "\n\n\tWindow " << e.window.windowID << " is offered focus.\n\n";
            break;
        case SDL_WINDOWEVENT_HIT_TEST:
            cout << "\n\n\tWindow " << e.window.windowID << " has a special hit test.\n\n";
            break;
        default:
            cout << "\n\n\tWindow " << e.window.windowID << " received an unknown event.\n\n";
            break;
        }
    }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void AsterCube::framebufferSize(int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    camera->resizeView(width, height);
}

void AsterCube::logSDLError(ostream &os, const string &msg)
{
    os << "\n\n\t" << msg << " error: " << SDL_GetError() << "\n\n";
}

