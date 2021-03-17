/*******************************************************************
 * Camera:  A class to contain the functions of an OpenGL camera.
 * Adapted from a class proposed on www.learnopengl.comp.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * April 2020 San Diego, California USA
 * ****************************************************************/

#include "../include/camera.h"

    // Constructor with vectors
Camera::Camera(int width, int height, vec3 position, vec3 focus) 
{
    cout << "\n\n\tCreating Camera.\n\n";
    Width = this->width = width;
    Height = this->height = height;
    this->position = Position = position;
    this->focus = Focus = focus;
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Zoom = ZOOM;
    WorldUp = WORLDUP;
    getEulerAngles();
    yaw = Yaw;
    pitch = Pitch;
    front = Front;
    if (debug1)
    {
        cout << "\n\t Constructor : Vectors : Yaw:  " 
        << Yaw << "  Pitch:  " << Pitch
        << " Position:  " << Position.x << ", " 
        << Position.y << ", " << Position.z
        << " Focus:  " << Focus.x << ", " 
        << Focus.y << ", " << Focus.z
        << " Front:  " << Front.x << ", " 
        << Front.y << ", " << Front.z;
    }
}

Camera::Camera(int width, int height, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
    cout << "\n\n\tCreating Camera.\n\n";
    Width = this->width = width;
    Height = this->height = height;
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Zoom = ZOOM;
    position = Position = vec3(posX, posY, posZ);
    WorldUp = vec3(upX, upY, upZ);
    this->yaw = Yaw = yaw;
    this->pitch = Pitch = pitch;
    getFront();
    focus = Focus;
    front = Front;
    
}

Camera::~Camera()
{
    cout << "\n\n\tDestroying Camera.\n\n";
}

vec3 Camera::getPosition()
{
    return Position;
}

void Camera::setPosition(vec3 position)
{
    Position = position;
}

void Camera::reverseDirection()
{
    cout << "\n\n\treverseDirection()  Yaw:  " << Yaw;
    Yaw += pi180;
    Yaw = remainder(Yaw, pi360);
    if(isnan(Yaw))
    {
        Yaw = 0.0f;
    }
    if (debug1)
    {
        cout << "\n\t reverseDirection() Vectors : Yaw:  " 
        << Yaw << "  Pitch:  " << Pitch
        << " Position:  " << Position.x << ", " 
        << Position.y << ", " << Position.z
        << " Focus:  " << Focus.x << ", " 
        << Focus.y << ", " << Focus.z
        << " Front:  " << Front.x << ", " 
        << Front.y << ", " << Front.z;
    }
    getFront();
}

void Camera::resizeView(int width, int height)
{
    this->Width = width;
    this->Height = height;
}    
mat4 Camera::getViewMatrix()
{
    if (debug1)
    {
        cout << "\n\t getViewMatrix() Vectors : Yaw:  " 
        << Yaw << "  Pitch:  " << Pitch
        << " Position:  " << Position.x << ", " 
        << Position.y << ", " << Position.z
        << " Focus:  " << Focus.x << ", " 
        << Focus.y << ", " << Focus.z
        << " Front:  " << Front.x << ", " 
        << Front.y << ", " << Front.z;
    }
    return lookAt(Position, Position + Front, Up);
}

mat4 Camera::getPerspective()
{
    return perspective(Zoom, (float)Width / (float)Height, 0.1f, 10000.0f);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    else if (direction == BACKWARD)
        Position -= Front * velocity;
    else if (direction == LEFT)
        Position -= Right * velocity;
    else if (direction == RIGHT)
        Position += Right * velocity;
    else if (direction == UP)
        Position += WorldUp * velocity;
    else if (direction == DOWN)
        Position -= WorldUp * velocity;
    else if (direction == CLOSER)
        Zoom -= 5.0f;
    else if (direction == AWAY)
        Zoom += 5.0f;
    if(true)
    {
        cout << "\n\tSpeed: " << MovementSpeed << " Time " << deltaTime 
        << " velocity " << velocity;
        cout << "\n\tPosition:  " << Position.x << ", " 
        << Position.y << ", " << Position.z;    
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;
    if (debug1)
    {
        cout << "\n\tMouse Offset Before x, y:  " << xoffset 
        << ", " << yoffset << " Yaw:  " << Yaw 
        << " Pitch:  " << Pitch;
    }
    Yaw   -= xoffset;
    Pitch += yoffset;
    if (debug1)
    {
        cout << "\n\tMouse Offset After x, y:  " << xoffset 
        << ", " << yoffset << " Yaw:  " << Yaw 
        << " Pitch:  " << Pitch;
    }
    // Update Front, Right and Up Vectors using the updated Euler angles
    getFront();
}

void Camera::processMouseScroll(Camera_Movement inout)
{
    if (inout == AWAY)
    {
        Zoom += onedegree;
    }
    if (inout == CLOSER)
    {
        Zoom -= onedegree;
    }
    if(Zoom <= onedegree)
    {
        Zoom = onedegree;
    }
    if(Zoom >= pi90)
    {
        Zoom = pi90;
    }
}

void Camera::resetCamera()
{
    cout << "\n\n\tCreating Camera.\n\n";
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Up = WorldUp = WORLDUP;
    Zoom = ZOOM;
    Position = position;
    Focus = focus;
    Front = front;
    Yaw = yaw;
    Pitch = pitch;
    
}    

void Camera::getEulerAngles()
{
    vec3 xzVec;
    // Calculate the new Front vector
    Front = normalize(Focus - Position);
    Pitch = asin(Front.y);
    // Also re-calculate the Right and Up vector
    Right = normalize(cross(Front, WorldUp));  // Normalize the vectors.
    Up    = normalize(cross(Right, Front));
    xzVec = normalize(cross(WORLDUP, Right));
    Yaw = asin(xzVec.z);
    if (debug1)
    {
        cout << "\n\t getEulerAngles() Vectors : Yaw:  " 
        << Yaw << "  Pitch:  " << Pitch
        << " Position:  " << Position.x << ", " 
        << Position.y << ", " << Position.z
        << " Focus:  " << Focus.x << ", " 
        << Focus.y << ", " << Focus.z
        << " Front:  " << Front.x << ", " 
        << Front.y << ", " << Front.z
        << " xzVec:  " << xzVec.x << ", "
        << xzVec.y << ", " << xzVec.z;
    }
}

void Camera::getFront()
{
    vec3 xzVec;
    vec3 front;
    front.x = cos(Yaw) * cos(Pitch);
    front.y = sin(Pitch);
    front.z = sin(Yaw) * cos(Pitch);
    Front = normalize(front);
    Focus = Front;
    // Also re-calculate the Right and Up vector
    Right = normalize(cross(Front, WorldUp));  // Normalize the vectors.
    Up    = normalize(cross(Right, Front));
    xzVec = normalize(cross(WORLDUP, Right));
    if (debug1)
    {
        cout << "\n\tgetFront() Vectors : Yaw:  " 
        << Yaw << "  Pitch:  " << Pitch
        << " Position:  " << Position.x << ", " 
        << Position.y << ", " << Position.z
        << " Focus:  " << Focus.x << ", " 
        << Focus.y << ", " << Focus.z
        << " Front:  " << Front.x << ", " 
        << Front.y << ", " << Front.z
        << " xzVec:  " << xzVec.x << ", "
        << xzVec.y << ", " << xzVec.z;
    }
}    
