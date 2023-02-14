#ifndef __DVEC3_H__
#define __DVEC3_H__

#include "defines.h"

#include <math.h>

#include "DVec2.h"


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



#endif