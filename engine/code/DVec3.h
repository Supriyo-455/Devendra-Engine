#ifndef __DVEC3_H__
#define __DVEC3_H__

#include "defines.h"

#include <math.h>

#include "DVec2.h"

// TODO: Alternative way to compute projection of a 3D vector
// TODO: Cross product

// TODO(supriyo): Figure out way to use xy to fill x and y coords together
typedef union vec3
{
    real32 E[3];
    struct
    {
        real32 x, y, z;
    };
}vec3;

inline vec3 vec(real32 x, real32 y, real32 z)
{
    vec3 V = {};
    V.x = x;
    V.y = y;
    V.z = z;
    return V;
}

inline vec3 vec(vec2 A, real32 z)
{
    vec3 V = {};
    V.x = A.x;
    V.y = A.y;
    V.z = z;
    return V;
}

inline vec3 operator+(vec3 A, vec3 B)
{
    vec3 Result = {};
    for(int i=0; i<3; i++)
    {
        Result.E[i] = A.E[i] + B.E[i];
    }
    return Result;
}

inline vec3 operator-(vec3 A, vec3 B)
{
    vec3 Result = {};
    for(int i=0; i<3; i++)
    {
        Result.E[i] = A.E[i] - B.E[i];
    }
    return Result;
}

inline vec3 operator*(real32 a, vec3 V)
{
    vec3 Result = {};
    for(int i=0; i<3; i++)
    {
        Result.E[i] = V.E[i] * a;
    }
    return Result;
}

inline real32 magnitude(vec3 A)
{
    real32 Result = 0.0f;
    for(int i=0; i<3; i++)
    {
        Result += A.E[i] * A.E[i];
    }
    Result = powf(Result, 0.5f);
    return Result;
}

inline vec3 normalize(vec3 A)
{
    real32 mag = magnitude(A);
    vec3 Result = {};
    for(int i=0; i<3; i++)
    {
        Result.E[i] = A.E[i] / mag;
        
    }
    return Result;
}

inline real32 dot(vec3 A, vec3 B)
{
    real32 Result = 0.0f;
    for(int i=0; i<3; i++)
    {
        Result += A.E[i] * B.E[i];
    }
    return Result;
}

inline vec3 cross(vec3 A, vec3 B)
{
    vec3 Result = {};
    Result.x = A.y * B.z - A.z * B.y;
    Result.y = A.z * B.x - A.x * B.z;
    Result.z = A.x * B.y - A.y * B.x;
    return Result;
}

inline vec3 projection(vec3 P, vec3 Q)
{
    vec3 Result = {};
    real32 Length = dot(P, Q) / magnitude(Q);
    vec3 Qnorm = normalize(Q);
    Result = Length * Qnorm;
    return Result;
}

inline vec3 perpendicular(vec3 P, vec3 Q)
{
    vec3 Result = {};
    Result = P - projection(P, Q);
    return Result;
}

#endif