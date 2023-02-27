#pragma once

#include "Devendra_Math.h"

struct ray
{
    vec3 origin;
    vec3 direction;
    void move(real32 t);
};

struct hit_record
{
    vec3 point;
    vec3 normal;
    real32 t;
    bool32 frontFace;

    inline void setFaceNormal(ray &Ray, vec3 &OutNormal);
};