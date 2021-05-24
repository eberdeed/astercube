/*******************************************************************
 * Camera:  A class to contain the functions of an OpenGL camera.
 *  Adapted from a class proposed on www.learnopengl.com.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * April 2020 San Diego, California USA
 * ****************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#include "commonheader.h"

/** \class Camera
 * A camera class that processes input and calculates the 
 * corresponding Euler Angles, Vectors and Matrices for moving the 
 * camera about a 3-dimensional landscape. For use in OpenGL.  This 
 * class relies heavily on GLM the OpenGL Math Library.
 */
class Camera
{
public:
    /* Enumeration */
    /** \brief Defines several possible options for camera movement. Used as 
     * abstraction to stay away from window-system specific input methods.
     * To access this from the calling class use 
     * "Camera::Camera_Movement::FORWARD," etc.
     */
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        CLOSER,
        AWAY,
        UP,
        DOWN,
    };
    /* Constants */
    //! Default camera values
    const float onedegree = acos(-1.0f) / 180.0f;  //! acos(-1) = pi.
    const float pi45 = acos(-1.0f) / 4.0f;
    const float pi90 = acos(-1.0f) / 2.0f;
    const float pi180 = acos(-1.0f);
    const float pi360 = acos(-1.0f) * 2.0f;
    const float YAW         = pi90;
    const float PITCH       =  0.0f;
    const float SPEED       =  0.05f;
    const float SENSITIVITY =  0.005f;
    const float ZOOM        =  pi45;
    //! Note that up in this world is -y.
    const vec3 WORLDUP      =  vec3(0.0f, -1.0f, 0.0f);
    //! Radian values to calculate location.
    /* Variables  */
    //! Camera Attributes
    vec3 Position, Front, Up, Focus, Right, WorldUp;
    //! Persistent camera attributes.
    vec3 position, focus, front;
    //! Missile position attributes.
    vec3 upStart, rightStart, frontStart, missilePos;
    float yawStart, pitchStart;
    //! Euler Angles: Large is current and small is persistent.
    float Yaw, yaw, Pitch, pitch;
    //! Camera options
    //! Adjust this to account for timing of the key movement.
    float MovementSpeed;
    //! Adjust this to account for the mouse movement.
    float MouseSensitivity;
    //! Zoom for a more detailed view of an object.
    float Zoom;
    //! Viewport dimensions.
    int Width, Height, width, height;
    //! Perspective matrix.
    mat4 projection = mat4(1.0f);
    //! Missile orientation.
    mat4 rotateMissile = mat4(1.0f);
    //! Debug flag. Setting this to true will give debug data on the console.
    bool debug1 = false;
    /* Functions  */
    /** \brief Constructor with vectors to define viewer position and focus.
     * width : the width of the viewport.
     * height : the height of the viewport.
     * position : the position of the camera.
     * focus :  the position of the object looked at.
     */
    Camera(int width, int height, vec3 position = vec3(0.0f, 0.0f, 2.0f), vec3 focus = vec3(0.0f, 0.0f, 0.0f)); 
    /** \brief Constructor with values to define viewer position and Euler angles for direction.
     * width : the width of the viewport.
     * height : the height of the viewport.
     * posX, posY, posZ : the (x, y, z) position of the camera.
     * upX, upY, upZ : the (x, y, z) up direction of camera up.
     * yaw : yaw (right <-> left).
     * pitch : pitch (up <-> down).
     */
    Camera(int width, int height, float posX, float posY, float posZ, float upX, float upY, 
           float upZ, float yaw, float pitch);
    /** \brief Destructor to echo the destruction of the class.
     */
    ~Camera();
    /** \brief Returns the LookAt Matrix.
     */
    mat4 getViewMatrix();
    /** \brief Get the front vector.
     */
    vec3 getFrontVec();
    /**  \brief Returns the current perspective matrix.
     */
    mat4 getPerspective();
    /** \brief Allows for viewport resize.
     */
    void resizeView(int width, int height);
    /** \brief Return the camera to its original settings.
     */
    void resetCamera();
    /** \brief Get the camera position.
     */
    vec3 getPosition();
    /** \brief Get the missile position.
     */
    mat4 missilePosition(float timeVal, bool start);
    /** \brief Get the location and orientation of the 
     * ship position.
     */
    mat4 shipPosition();
    /** \brief Set the camera position.
     */
    void setPosition(vec3 position);
    /** \brief Set the front position.
     */
    void setFocus(vec3 focus);
    /** \brief Print a vec3.
     */
    void printVec3(vec3 vecVal);
    /** \brief Print a mat4.
     */
    void printMat4(mat4 matVal);
    /** Multiply two mat4 matrices.
     */
    mat4 multMat4(mat4 itemone, mat4 itemtwo);
    /** \brief Rotate the camera 180 degrees
     *  on the XZ plane.
     */
    void reverseDirection();
    /** \brief Processes input received from any keyboard-like input system. 
     * Accepts input parameter in the form of camera defined ENUM
     * such as Camera::Camera_Movement::RIGHT or Camera::Camera_Movement::UP 
     * (to abstract it from windowing systems). This is adjustable using
     * the SPEED variable.
     */
    void processKeyboard(Camera_Movement direction, float deltaTime);
    
    /** \brief Processes input received from a mouse input system. 
     * Expects the offset value in both the x and y direction.
     * This is adjustable using the SENSITIVITY variable.
     */
    void processMouseMovement(float xoffset, float yoffset);
    /** \brief Processes input received from a mouse scroll-wheel event. 
     * Only requires input on the wheel-axis
     */
    void processMouseScroll(Camera_Movement inout);
    /** \brief Calculates the Euler angles from the front vector.
     */
    void getEulerAngles();
    /** \brief Calculate the front vector from the Euler angles.
     */
    void getFront();
   
};

#endif // CAMERA_H
