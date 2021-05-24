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
    Up = WorldUp = WORLDUP;
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

mat4 Camera::missilePosition(float timeVal, bool start)
{
    vec3 loc;
    mat4 translate = mat4(1.0f); // Identity matrix by default
    getFront();
    if (start)
    {
        if (debug1)
        {
            cout << "\n\tMissile yaw, pitch: " << Yaw << ", " << Pitch;
        }
        yawStart = Yaw - pi90;
        pitchStart = Pitch + pi90;
        frontStart = Front;
        upStart = Up;
        rightStart = -Right;
        missilePos = Position + (3.0f * Front) + vec3(0.0f, 0.3f, 0.0f);
        rotateMissile = mat4(1.0f);
        rotateMissile = glm::rotate(rotateMissile, pi180, upStart);
        rotateMissile = glm::rotate(rotateMissile, pitchStart, rightStart);
    }
    // Calculate the new Front vector
    missilePos += frontStart * timeVal * 3.0f;
    if (debug1)
    {
        cout << "\n\tStart " << start << " yaw and pitch " << yawStart << ", " << pitchStart
        << " time " << timeVal << " front ";
        printVec3(frontStart);
        cout << "\n\tMissile Position: ";
        printVec3(missilePos);
        cout << "\n\tMissile Up: ";
        printVec3(upStart);
    }
    // Create translation and rotation matrix
    translate = glm::translate(translate, missilePos);
    return(multMat4(translate, rotateMissile)); 
}
    
    
mat4 Camera::shipPosition()
{
    vec3 shipPos = (Position + (3.0f * Front));
    if (debug1)
    {
        cout << "\n\tPosition ";
        printVec3(Position);
        cout << "  Ship Position ";
        printVec3(shipPos);
    }
    getFront();
    // Create translation and rotation matrix
    mat4 translate = mat4(1.0f); // Identity matrix by default
    translate = glm::translate(translate, shipPos);
    mat4 rotate = mat4(1.0f);
    if (debug1)
    {
        cout << "\n\tShip yaw, pitch: " << Yaw << ", " << Pitch;
    }
    rotate = glm::rotate(rotate, pi180, Front);
    rotate = glm::rotate(rotate, Yaw + pi180, Up);
    rotate = glm::rotate(rotate, Pitch, -Right);
    return(multMat4(translate, rotate)); 
}

mat4 Camera::multMat4(mat4 itemone, mat4 itemtwo)
{
    mat4 result;
    float sum = 0.0f;
    for (int z = 0; z < 4; z++)
    {
        for (int x = 0; x < 4; x++)
        {
            sum = 0.0f;
            for (int y = 0; y < 4; y++)
            {
                sum += itemtwo[z][y] * itemone[y][x];
            }
            result[z][x] = sum;
        }
    }
    return result;
}

void Camera::setPosition(vec3 position)
{
    Position = position;
    if (debug1)
    {
        cout << "\n\tCamera View Position: " << position.x << ", " << position.y
        << ", " << position.z;
    }
}

void Camera::setFocus(vec3 focus)
{
    Focus = focus;
    if (debug1)
    {
        cout << "\n\tCamera Focus Position: " << focus.x << ", " << focus.y << ", "
        << focus.z;
    }
    getEulerAngles();
}

void Camera::reverseDirection()
{
    if (debug1)
    {
        cout << "\n\n\treverseDirection()  Yaw:  " << Yaw;
    }
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

vec3 Camera::getFrontVec()
{
    if (debug1)
    {
        cout << "\n\tFront ";
        printVec3(Front);
    }
    return Front;
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
    if(debug1)
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
    glm::clamp(Yaw, -pi180, pi180);
    glm::clamp(Pitch, -pi180, pi180);
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
    glm::clamp(Pitch, -pi90, pi90);
    if (debug1)
    {
        cout << "\n\tFront: " << Front.x << ", "
        << Front.y << ", " << Front.z 
        << "  Pitch: " << Pitch;
    }
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
    if (debug1)
    {
        cout << "\n\tYaw " << Yaw << " Pitch " << Pitch << " Front ";
        printVec3(front);
    }
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
void Camera::printVec3(vec3 vecVal)
{
    cout << "  3 Float Vector:  ";
    for  (int x = 0; x < 3; x++)
    {
        cout << vecVal[x] << ", ";
    }
}

void Camera::printMat4(mat4 matVal)
{
    cout << "  4x4 Matrix\n\t";
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            cout << fixed << setprecision(4) << setw(8) << matVal[y][x] << ", ";
        }
        cout << "\n\t";
    }
}

