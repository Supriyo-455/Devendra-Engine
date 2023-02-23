#pragma once

#include "defines.h"
#include "Devendra_Math.h"
#include <iostream>
#include <vector>

struct ray
{
    vec3 origin;
    vec3 direction;
};

struct hit_record 
{
    vec3 point;
    vec3 normal;
    real32 t;
    bool32 frontFace;
};

struct sphere
{
    vec3 Center;
    real32 Radius;
};

struct world
{
    // TODO: Replace vector in future?
    std::vector<sphere*> Spheres;
};

inline void WorldAddSpherers(world* World, sphere Sphere)
{
    World->Spheres.push_back(&Sphere);
}

inline vec3 RayAt(const ray* Ray, real32 t)
{
    return Ray->origin + (t * Ray->direction);
}

inline sphere CreateSphere(vec3 Center, real32 Radius)
{
    sphere Sphere = {};
    Sphere.Center = Center;
    Sphere.Radius = Radius;
    return Sphere;
}

inline void SetFaceNormal(hit_record* HitRecord, const ray* Ray, vec3 OutNormal)
{
    HitRecord->frontFace = dot(Ray->direction, OutNormal) < 0;
    HitRecord->normal = HitRecord->frontFace ? OutNormal : (-1.0f * OutNormal);
}

inline bool32 HitSphere(const sphere* Sphere, const ray* Ray, hit_record* HitRecord, real32 tmin, real32 tmax)
{
    vec3 CenterToRayOrigin = Ray->origin - Sphere->Center;
    real32 a = dot(Ray->direction, Ray->direction);
    real32 half_b = dot(Ray->direction, CenterToRayOrigin);
    real32 c = dot(CenterToRayOrigin, CenterToRayOrigin) - Sphere->Radius*Sphere->Radius;

    real32 Discriminant = half_b*half_b - a*c;
    
    if(Discriminant < 0.0f) {
        return false;
    }
    real32 sqrtDis = sqrtf(Discriminant);

    // Note: Finding nearest root available in the acceptable range
    real32 Root = (-half_b - sqrtDis) / a;
    if(Root < tmin || tmax < Root)
    {
        Root = (-half_b + sqrtDis) / a;
        if(Root < tmin || tmax < Root)
            return false;
    }

    HitRecord->t = Root;
    HitRecord->point = RayAt(Ray, HitRecord->t);
    vec3 OutNorm = (HitRecord->point - Sphere->Center) / Sphere->Radius;
    SetFaceNormal(HitRecord, Ray, OutNorm);

    return true;
}

inline bool32 HitWorld(world *World, const ray* Ray, hit_record* HitRecord, real32 tmin, real32 tmax)
{
    hit_record TempRecord = {};
    bool32 HitAnything = false;
    real32 ClosestSoFar = tmax;

    // For spheres
    for(sphere* Sphere: World->Spheres)
    {
        if(HitSphere(Sphere, Ray, &TempRecord, tmin, ClosestSoFar))
        {
            HitAnything = true;
            ClosestSoFar = TempRecord.t;
            *HitRecord = TempRecord;    // NOTE: Very critical for pointing to local variable in C!!
        }
    }

    return HitAnything;
}

inline vec3 RayColor(const ray* Ray, world* World)
{   
    hit_record HitRecord = {};
    if(HitWorld(World, Ray, &HitRecord, 0.0f, Inf))
    {
        return 0.5f * (HitRecord.normal + vec(1.0f, 1.0f, 1.0f));
    }
    vec3 UnitDirection = normalize(Ray->direction);
    real32 t = (real32)0.5f*(UnitDirection.y + 1.0f);
    vec3 color1 = vec(1.0f, 1.0f, 1.0f);
    vec3 color2 = vec(0.5f, 0.7f, 1.0f);
    return lerp(color1, color2, t);
}