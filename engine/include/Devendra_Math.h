#ifndef __DEVENDRA_MATH_H__
#define __DEVENDRA_MATH_H__

#include "defines.h"
#include <math.h>

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

typedef union vec2
{
    real32 E[2];
    struct
    {
        real32 x, y;
    };
}vec2;

// TODO(supriyo): Figure out way to use xy to fill x and y coords together
typedef union vec3
{
    real32 E[3];
    struct
    {
        real32 x, y, z;
    };
}vec3;

// TODO(supriyo): Figure out way to use xyz to fill x , y and z coords together
typedef union vec4
{
    real32 E[4];
    struct
    {
        real32 x, y, z, w;
    };
}vec4;

inline vec2 vec(real32 x, real32 y)
{
    vec2 V = {};
    V.x = x;
    V.y = y;
    return V;
}

inline vec3 vec(real32 x, real32 y, real32 z)
{
    vec3 V = {};
    V.x = x;
    V.y = y;
    V.z = z;
    return V;
}

inline vec4 vec(real32 x, real32 y, real32 z, real32 w)
{
    vec4 V = {};
    V.x = x;
    V.y = y;
    V.z = z;
    V.w = w;
    return V;
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

inline real32 angle(vec2 A, vec2 B)
{
    vec2 normA = normalize(A);
    vec2 normB = normalize(B);
    real32 cosValue = dot(normA, normB);
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


// Matrix operations

typedef struct mat4x4
{
    real32 E[4][4];
}mat4x4;

inline mat4x4 identity()
{
    mat4x4 R = {};

    for(int i=0; i<4; i++)
    {
        R.E[i][i] = 1;
    }
    return R;
}

inline mat4x4 scalingMat4x4(vec3 S)
{
    mat4x4 I = identity();

    for(int i=0; i<3; i++)
    {
        I.E[i][i] *= S.E[i];
    }
    return I;
}

inline mat4x4 transMat4x4(mat4x4 A, vec3 T)
{
    for(int i=0; i<3; i++)
    {
        A.E[i][3] = T.E[i];
    }
    return A;
}

inline mat4x4 operator*(mat4x4 A, real32 B)
{
    mat4x4 R = {};
    for(int r=0; r<4; r++)
    {
        for(int c=0; c<4; c++)
        {
            R.E[r][c] = B*A.E[r][c];
        }
    }
    return R;
}

inline mat4x4 operator*(mat4x4 A, mat4x4 B)
{
    mat4x4 R = {};
    for(int r=0; r<4; r++)
    {
        for(int c=0; c<4; c++)
        {
            for(int i=0; i<4; i++)
            {
                R.E[r][c] += A.E[r][i] + B.E[i][c];
            }
        }
    }
    return R;
}

inline vec4 operator*(mat4x4 A, vec4 B)
{
    vec4 R = {};
    for(int r=0; r<4; r++)
    {
        for(int c=0; c<4; c++)
        {
            R.E[r] += A.E[r][c] * B.E[c]; 
        }
    }
    return R;
}

inline mat4x4 operator+(mat4x4 A, mat4x4 B)
{   
    mat4x4 R = {};
    for(int r=0; r<4; r++)
    {
        for(int c=0; c<4; c++)
        {
            R.E[r][c] += A.E[r][c] + B.E[r][c]; 
        }
    }
    return R;
}

inline mat4x4 rotateZ(real32 deg)
{   
    real32 sinval = sin(deg);
    real32 cosval = cos(deg);
    mat4x4 rMat = 
    {
        {
            {cosval,   -sinval,   0,    0},
            {sinval,   cosval,    0,    0},
            {0,             0,    1,    0},
            {0,             0,    0,    1}
        },
    };
    return rMat;
}

inline mat4x4 rotateX(real32 deg)
{   
    real32 sinval = sin(deg);
    real32 cosval = cos(deg);
    mat4x4 rMat = 
    {
        {
            {1,        0,          0,     0},
            {0,   cosval,    -sinval,     0},
            {0,   sinval,     cosval,     0},
            {0,        0,          0,     1}
        },
    };
    return rMat;
}

inline mat4x4 transpose(mat4x4 A)
{
    mat4x4 R;
    for(int j=0; j<4; j++)
    {
        for(int i=0; i<4; i++)
        {
            R.E[j][i] = A.E[i][j];
        }
    }
    return R;
}

inline mat4x4 projection(real32 AspectWithOverHeight, real32 FocalLength)
{   
    real32 a = 1.0f;
    real32 b = AspectWithOverHeight;
    real32 c = FocalLength;
    mat4x4 R = 
    {
        {
            {a, 0, 0, 0},
            {0, b, 0, 0},
            {0, 0, 1, 0},
            {0, 0, c, 0}
        }
    };

    return R;
}

#endif