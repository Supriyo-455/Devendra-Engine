#pragma once

#include "defines.h"
#include <math.h> // TODO: Remove this in future

#include <cstdlib>

#define Inf INFINITY

int32 Fact(int32 n)
{
    return n <= 0 ? 1 : n * Fact(n - 1);
}

inline real32 Absf(real32 x)
{
    if (x < 0)
        x = -x;
    return x;
}

inline real32 Sin(real32 x)
{
    real32 sign = 1;
    if (x < 0)
    {
        sign = -1.0;
        x = -x;
    }
    if (x > 360)
        x -= int32(x / 360) * 360;
    x *= PI / 180.0;
    real32 res = 0;
    real32 term = x;
    int32 k = 1;
    while (res + term != res)
    {
        res += term;
        k += 2;
        term *= -x * x / k / (k - 1);
    }

    return sign * res;
}

inline real32 Cos(real32 x)
{
    if (x < 0)
        x = -x;
    if (x > 360)
        x -= int32(x / 360) * 360;
    x *= PI / 180.0;
    real32 res = 0;
    real32 term = 1;
    int32 k = 0;
    while (res + term != res)
    {
        res += term;
        k += 2;
        term *= -x * x / k / (k - 1);
    }
    return res;
}

inline real32 lerp(real32 v0, real32 v1, real32 t)
{
    return (1 - t) * v0 + t * v1;
}

inline real32 DegreesToRadians(real32 Degree)
{
    return Degree * PI / 180.0f;
}

inline real32 Random()
{
    return rand() / (RAND_MAX + 1.0f);
}

inline real32 Random(real32 min, real32 max)
{
    return min + (max - min) * Random();
}

inline real32 Clamp(real32 x, real32 min, real32 max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

#include "DVec2.h" // NOTE: For vector 2D operations

#include "DVec3.h" // NOTE: For vector 3D operations

#include "DVec4.h" // NOTE: For vector 4D operations

#include "DMatrix.h" // NOTE: For matrix operations