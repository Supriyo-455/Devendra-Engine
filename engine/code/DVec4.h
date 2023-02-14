#ifndef __DVEC4_H__
#define __DVEC4_H__


#include "defines.h"

#include <math.h>

#include "DVec2.h"


// TODO(supriyo): Figure out way to use xyz to fill x , y and z coords together
typedef union vec4
{
    real32 E[4];
    struct
    {
        real32 x, y, z, w;
    };
}vec4;

inline vec4 vec(real32 x, real32 y, real32 z, real32 w)
{
    vec4 V = {};
    V.x = x;
    V.y = y;
    V.z = z;
    V.w = w;
    return V;
}

#endif