#ifndef __DEVENDRA_MATH_H__
#define __DEVENDRA_MATH_H__

#include "defines.h"
#include <math.h>   // for frexpf, ldexpf, isinf, nextafterf

#define PI 3.1415926535897f

int32 fact(int32 n) {
    return n <= 0 ? 1 : n * fact(n-1);
}

inline real32 absf(real32 x)
{
    if(x < 0) x = -x;
    return x;
}

inline real32 sin(real32 x){
    real32 sign=1;
    if (x<0){
        sign=-1.0;
        x=-x;
    }
    if (x>360) x -= int32(x/360)*360;
    x*=PI/180.0;
    real32 res=0;
    real32 term=x;
    int32 k=1;
    while (res+term!=res){
        res+=term;
        k+=2;
        term*=-x*x/k/(k-1);
    }

    return sign*res;
}

inline real32 cos(real32 x){
    if (x<0) x=-x;
    if (x>360) x -= int32(x/360)*360;
    x*=PI/180.0;
    real32 res=0;
    real32 term=1;
    int32 k=0;  
    while (res+term!=res){
        res+=term;
        k+=2;
        term*=-x*x/k/(k-1);
    }  
    return res;
}

union vec2
{
    real32 E[2];
    struct
    {
        real32 x, y;
    };
};

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
        result += A[i] * B[i];
    }
    return result;
}

inline real32 angle(vec2 A, vec2 B)
{
    real32 magA = magnitude(A);
    real32 magB = magnitude(B);
    real32 cosValue = dot(A/magA, B/magB);
    return acosf(cosValue);
}

inline vec2 operator-(vec2 A)
{
    vec2 Result;
    Result.x = -A.x;
    Result.y = -A.y;
    return Result;
}

inline vec2 operator+(vec2 A, vec2 B)
{
    vec2 Result;
    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    return Result;
}

inline vec2 operator-(vec2 A, vec2 B)
{
    vec2 Result;
    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    return Result;
}

inline vec2 operator*(real32 A, vec2 B)
{
    vec2 Result;
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

#endif