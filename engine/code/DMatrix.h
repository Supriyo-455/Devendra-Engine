#ifndef __DMATRIX_H__
#define __DMATRIX_H__

#include <math.h>
#include "defines.h"
#include "DVec2.h"
#include "DVec3.h"
#include "DVec4.h"

/*
    TODO: 
        1) Matrix transpose
        2) Orthogonal vectors check
        3) Projection of vector
        4) Normalization of vector
        5) power(double, double) without math.h

*/

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