#ifndef __DVEC2_H__
#define __DVEC2_H__

#include "defines.h"

#include <math.h>

// TODO: Cross product

typedef union vec2
{
    real32 E[2];
    struct
    {
        real32 x, y;
    };
}vec2;

inline vec2 vec(real32 x, real32 y)
{
    vec2 V = {};
    V.x = x;
    V.y = y;
    return V;
}

inline vec2 operator-(vec2 A)
{
    vec2 Result = {};
    Result.x = -A.x;
    Result.y = -A.y;
    return Result;
}

inline vec2 operator+(vec2 A, vec2 B)
{
    vec2 Result = {};
    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    return Result;
}

inline vec2 operator-(vec2 A, vec2 B)
{
    vec2 Result = {};
    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    return Result;
}

inline vec2 operator*(real32 A, vec2 B)
{
    vec2 Result = {};
    Result.x = A*B.x;
    Result.y = A*B.y;
    return Result;
}

inline vec2& operator*=(vec2& B, real32 A)
{
    B = A * B;
    return B;
}

inline vec2& operator+=(vec2 A, vec2 B)
{
    A = B + A;
    return A;
}

inline real32 magnitude(vec2 A)
{
    real32 x2 = A.x * A.x;
    real32 y2 = A.y * A.y;
    return powf((x2 + y2), 0.5f);
}

inline vec2 normalize(vec2 A)
{
    real32 mag = magnitude(A);
    A.x /= mag;
    A.y /= mag;
    return A;
}

inline real32 dot(vec2 A, vec2 B)
{
    real32 result = 0.0f;
    for(int i=0; i<2; i++)
    {
        result += A.E[i] * B.E[i];
    }
    return result;
}

inline vec2 projection(vec2 P, vec2 Q)
{
    vec2 Result = {};
    real32 length = dot(P, Q) / magnitude(Q);
    vec2 Qnorm = normalize(Q);
    Result = length * Qnorm;
    return Result;
}

inline vec2 perpendicular(vec2 P, vec2 Q)
{
    vec2 Result = {};
    Result = P - projection(P, Q);
    return Result;
}

inline real32 angle(vec2 A, vec2 B)
{
    vec2 normA = normalize(A);
    vec2 normB = normalize(B);
    real32 cosValue = dot(normA, normB);
    return acosf(cosValue);
}

#endif