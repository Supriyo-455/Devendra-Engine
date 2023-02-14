#pragma once

#include "defines.h"
#include <math.h>   // TODO: Remove this in future


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

#include "DVec2.h"  // NOTE: For vector 2D operations

#include "DVec3.h"  // NOTE: For vector 3D operations

#include "DVec4.h"  // NOTE: For vector 4D operations

#include "DMatrix.h"    // NOTE: For matrix operations