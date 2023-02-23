#ifndef __DMATRIX_H__
#define __DMATRIX_H__

#include <math.h>
#include "defines.h"
#include "DVec2.h"
#include "DVec3.h"
#include "DVec4.h"

/*
    TODO:


*/

typedef union mat4x4
{
    real32 E[4][4];
    struct
    {
        vec4 a, b, c, d;
    };
}mat4x4;

typedef union mat3x3
{
    real32 E[3][3];
    struct
    {
        vec3 a, b, c;
    };
}mat3x3;

inline mat3x3 identity3x3()
{
    mat3x3 R = {};

    for(int i=0; i<3; i++)
    {
        R.E[i][i] = 1;
    }
    return R;
}

inline mat3x3 operator*(mat3x3 mat, real32 f)
{
    mat3x3 R = {};
    for(int r=0; r<3; r++)
    {
        for(int c=0; c<3; c++)
        {
            R.E[r][c] = f * mat.E[r][c];
        }
    }
    return R;
}

inline mat4x4 identity4x4()
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
    mat4x4 I = identity4x4();

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
    real32 sign = -1.0f;
    sign = powf(sign, (real32)row+col);
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
    Result = Result * sign;
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
    return  mat.E[0][3] * mat.E[1][2] * mat.E[2][1] * mat.E[3][0] - mat.E[0][2] * mat.E[1][3] * mat.E[2][1] * mat.E[3][0] -
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
}

inline mat3x3 inverse(mat3x3 mat)
{
    mat3x3 Result = {};
    vec3 r0 = cross(mat.b, mat.c);
    vec3 r1 = cross(mat.c, mat.a);
    vec3 r2 = cross(mat.a, mat.b);

    real32 denom = 1.0f / dot(r2, mat.c);
    
    assert(denom != 0);

    Result.a = denom*r0;
    Result.b = denom*r1;
    Result.c = denom*r2;

    return Result;
}

// TODO: Bug here need to debug!
inline mat4x4 inverse(mat4x4 mat)
{
    vec3 a = vec(mat.E[0][0], mat.E[1][0], mat.E[2][0]);
    vec3 b = vec(mat.E[0][1], mat.E[1][1], mat.E[2][1]);
    vec3 c = vec(mat.E[0][2], mat.E[1][2], mat.E[2][2]);
    vec3 d = vec(mat.E[0][3], mat.E[1][3], mat.E[2][3]);

    real32 x = mat.E[3][0];
    real32 y = mat.E[3][1];
    real32 z = mat.E[3][2];
    real32 w = mat.E[3][3];

    vec3 s = cross(a, b);
    vec3 t = cross(c, d);
    vec3 u = (y * a) - (x * b);
    vec3 v = (w * c) - (z * d);

    real32 det = dot(s, v) + dot(t, u);
    real32 invDet = 1.0f / det;
    assert(invDet != 0);
    s = invDet * s;
    t = invDet * t;
    u = invDet * u;
    v = invDet * v;

    vec3 r0 = cross(b, v) + y * t;
    vec3 r1 = cross(v, a) - x * t;
    vec3 r2 = cross(d, u) + w * s;
    vec3 r3 = cross(u, c) - z * s;

    mat4x4 adj = {
        {
            {r0.x, r0.y, r0.z, -dot(b, t)},
            {r1.x, r1.y, r1.z,  dot(a, t)},
            {r2.x, r2.y, r2.z, -dot(d, s)},
            {r3.x, r3.y, r3.z,  dot(c, s)}
        }
    };

    return adj;
}

#endif