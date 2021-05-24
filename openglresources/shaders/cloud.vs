/*********************************************************
*   cloud.vs:  A vertex shader for a gaseous cloud display
*   using spherical coordinates converted to cartesian.
*   Created by:  Edward C. Eberle <eberdeed@eberdeed.net>
*   March 2020 San Diego, California USA
*   Copyright Maureen Ann Quinn March 2020
**********************************************************/
#version 300 es

//! The first three values are the spherical 
//! coordinates of the starting point of a particle.
//! Everything is a vec4 to ensure data integrity.
/*
data.x = phi1;
data.y = rho1;
data.z = radius1;
data.w = a_lifetime;
*/
layout(location = 0) in vec4 data;

/** The only thing passed out is the current
  * position in time travelling from 1 -> 0.
  */
out float v_lifetime;
/* Uniforms */
//! Current time (from zero).
uniform float u_time;
//! Explosion scaler.
uniform mat4 scaler;
//! Camera view matrix.
uniform mat4 view;
//! Model
uniform mat4 model;

vec4 tmpPos;
mat4 tmpMat;
float timeval, x, y, z, r, phi, 
phi1, rho, rho1, radius1, a_lifetime, theta;
float pi2 = 2.0 * acos(-1.0);

//! Floating point modulus.
float fmod(float num, float div);

void main()
{
    gl_PointSize = 3.0;
    phi1 = data.x;
    rho1 = data.y;
    radius1 = data.z;
    a_lifetime = data.w;
    //! Check to see if the explosion is still happening.
    if (u_time <= a_lifetime)
    {
        //! Create a ratio so the time goes from zero to one.
        timeval = (u_time / a_lifetime);
        //! slide the radius from zero to it's full value.
        r = radius1 * timeval;
        r = clamp(r, 0.0, 1.0);
        //!  Create a timeline that goes from one to zero.
        v_lifetime = 1.0 - timeval;
        //! Square that value.
        // v_lifetime *= v_lifetime;
        //! Clamp it between 0 and 1.
        // v_lifetime = clamp ( v_lifetime, 0.0, 1.0 );
        /** Make the point size vary inversly according to
          * the square of the time.
          */
        //! Make the particle slide along a circular path.
        theta  = timeval * pi2;
        theta = fmod(theta, pi2);
        //! The two angles describe a circle using
        //! sine and cosine.
        phi = cos(theta) * pi2 + phi1;
        rho = sin(theta) * pi2 + rho1;
        //! Convert the coordinates from spherical to cartesian.
        x = r * sin(phi) * cos(rho);
        y = r * sin(phi) * sin(rho);
        z = r * cos(phi);
        tmpPos = vec4(x, y, z, 1.0);
        //! Apply the projection, view and scale matrices.
        //gl_Position = scaler * model * view * projection * tmpPos;
        gl_Position = model * scaler * view * tmpPos;
    }
    else
    {
        //! Otherwise dump it in the corner.
        gl_Position = vec4( -1000.0, -1000.0, -1000.0, 0.0 );
    }
}
//! Floating point modulus.
float fmod(float num, float div)
{
    int result = int(floor(num / div));
    return(num - (float(result) * div));
}
