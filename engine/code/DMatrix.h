#ifndef __DMATRIX_H__
#define __DMATRIX_H__

#include <math.h>
#include "defines.h"
#include "DVec2.h"
#include "DVec3.h"
#include "DVec4.h"

/*
    TODO: 
            * Inverse of matrix

*/

typedef struct mat4x4
{
    real32 E[4][4];
}mat4x4;

typedef struct mat3x3
{
    real32 E[3][3];
}mat3x3;

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

inline bool operator==(mat4x4 A, mat4x4 B)
{
    for(int r=0; r<4; r++)
    {
        for(int c=0; c<4; c++)
        {
            if(A.E[r][c] != B.E[r][c]) return false;
        }
    }
    return true;
}

inline mat4x4 rotateZ(real32 deg)
{   
    real32 sinval = sinf(deg);
    real32 cosval = cosf(deg);
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
    real32 sinval = sinf(deg);
    real32 cosval = cosf(deg);
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

inline mat3x3 cofactor(mat4x4 mat, int row, int col)
{
    mat3x3 Result = {};
    int r = 0, c = 0;
    for(int i=0; i<4; i++)
    {
        if(i == row) continue;
        for(int j=0; j<4; j++)
        {
            if(j == col) continue;
            Result.E[r][c] = mat.E[i][j];
            c++;
        }
        r++;
    }
    return Result;
}

inline real32 det(mat3x3 mat)
{
    return (  
                mat.E[0][0] * (mat.E[1][1] * mat.E[2][2] - mat.E[1][2] * mat.E[2][1]) 
                + mat.E[0][1] * (mat.E[1][2] * mat.E[2][0] - mat.E[1][0] * mat.E[2][2])
                + mat.E[0][2] * (mat.E[1][0] * mat.E[2][1] - mat.E[1][1] * mat.E[2][0])
            );
}

inline real32 det(mat4x4 mat)
{
    real32 Result =  mat.E[0][3] * mat.E[1][2] * mat.E[2][1] * mat.E[3][0] - mat.E[0][2] * mat.E[1][3] * mat.E[2][1] * mat.E[3][0] -
         mat.E[0][3] * mat.E[1][1] * mat.E[2][2] * mat.E[3][0] + mat.E[0][1] * mat.E[1][3] * mat.E[2][2] * mat.E[3][0] +
         mat.E[0][2] * mat.E[1][1] * mat.E[2][3] * mat.E[3][0] - mat.E[0][1] * mat.E[1][2] * mat.E[2][3] * mat.E[3][0] -
         mat.E[0][3] * mat.E[1][2] * mat.E[2][0] * mat.E[3][1] + mat.E[0][2] * mat.E[1][3] * mat.E[2][0] * mat.E[3][1] +
         mat.E[0][3] * mat.E[1][0] * mat.E[2][2] * mat.E[3][1] - mat.E[0][0] * mat.E[1][3] * mat.E[2][2] * mat.E[3][1] -
         mat.E[0][2] * mat.E[1][0] * mat.E[2][3] * mat.E[3][1] + mat.E[0][0] * mat.E[1][2] * mat.E[2][3] * mat.E[3][1] +
         mat.E[0][3] * mat.E[1][1] * mat.E[2][0] * mat.E[3][2] - mat.E[0][1] * mat.E[1][3] * mat.E[2][0] * mat.E[3][2] -
         mat.E[0][3] * mat.E[1][0] * mat.E[2][1] * mat.E[3][2] + mat.E[0][0] * mat.E[1][3] * mat.E[2][1] * mat.E[3][2] +
         mat.E[0][1] * mat.E[1][0] * mat.E[2][3] * mat.E[3][2] - mat.E[0][0] * mat.E[1][1] * mat.E[2][3] * mat.E[3][2] -
         mat.E[0][2] * mat.E[1][1] * mat.E[2][0] * mat.E[3][3] + mat.E[0][1] * mat.E[1][2] * mat.E[2][0] * mat.E[3][3] +
         mat.E[0][2] * mat.E[1][0] * mat.E[2][1] * mat.E[3][3] - mat.E[0][0] * mat.E[1][2] * mat.E[2][1] * mat.E[3][3] -
         mat.E[0][1] * mat.E[1][0] * mat.E[2][2] * mat.E[3][3] + mat.E[0][0] * mat.E[1][1] * mat.E[2][2] * mat.E[3][3];
    return Result;
}

#endif