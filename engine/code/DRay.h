#pragma once

#include "defines.h"
#include "Devendra_Math.h"
#include <iostream>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

struct ray
{
    vec3 origin;
    vec3 direction;

    vec3 move(real32 t)
    {
        return this->origin + (t * this->direction);
    }
};

struct hit_record 
{
    vec3 point;
    vec3 normal;
    real32 t;
    bool32 frontFace;

    inline void setFaceNormal(ray& Ray, vec3& OutNormal)
    {
        this->frontFace = dot(Ray.direction, OutNormal) < 0;
        this->normal = this->frontFace ? OutNormal : (-1.0f * OutNormal);
    }
};

struct entity 
{
    virtual bool32 hit(ray& Ray, hit_record& HitRecord, real32 tmin, real32 tmax) = 0;
};

struct sphere : entity
{
    vec3 Center;
    real32 Radius;

    explicit sphere(vec3 Center, real32 Radius)
    {
        this->Center = Center;
        this->Radius = Radius;
    }

    virtual bool32 hit(ray& Ray, hit_record& HitRecord, real32 tmin, real32 tmax) override
    {
        vec3 CenterToRayOrigin = Ray.origin - this->Center;
        real32 a = dot(Ray.direction, Ray.direction);
        real32 half_b = dot(Ray.direction, CenterToRayOrigin);
        real32 c = dot(CenterToRayOrigin, CenterToRayOrigin) - this->Radius*this->Radius;

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

        HitRecord.t = Root;
        HitRecord.point = Ray.move(HitRecord.t);
        vec3 OutNorm = (HitRecord.point - this->Center) / this->Radius;
        HitRecord.setFaceNormal(Ray, OutNorm);

        return true;
    }
};

struct world : entity
{
    // TODO: Replace vector in future?
    std::vector<shared_ptr<entity>> Objects;

    void add(shared_ptr<entity> object)
    {
        this->Objects.push_back(object);
    }

    void clear() { 
        this->Objects.clear(); 
    }

    virtual bool32 hit(
        ray& Ray, 
        hit_record& HitRecord, 
        real32 tmin, 
        real32 tmax) override {

            hit_record TempRecord;
            bool32 HitAnything = false;
            real32 ClosestSoFar = tmax;

            for(const shared_ptr<entity>& Object: this->Objects)
            {
                if(Object->hit(Ray, TempRecord, tmin, ClosestSoFar))
                {
                    HitAnything = true;
                    ClosestSoFar = TempRecord.t;
                    HitRecord = TempRecord;    // NOTE: Very critical for pointing to local variable in C!!
                }
            }

            return HitAnything;
    }
};

struct camera
{
    vec3 Origin;
    vec3 LowerLeftCorner;
    vec3 Horizontal;
    vec3 Vertical;
    
    real32 ViewportHeight = 2.0f;
    real32 ViewportWidth = ViewportHeight;
    real32 FocalLength = 1.0f;
    
    explicit camera(real32 AspectRatio)
    {
        ViewportWidth *= AspectRatio;
        this->Origin = vec(0.0f, 0.0f, 0.0f);
        this->Horizontal = vec(ViewportWidth, 0.0f, 0.0f);
        this->Vertical = vec(0.0f, ViewportHeight, 0.0f);
        this->LowerLeftCorner = this->Origin - this->Horizontal/2 - this->Vertical/2 - vec(0, 0, FocalLength);
    }

    ray getRay(real32 u, real32 v)
    {
        ray Ray;
        Ray.origin = this->Origin;
        Ray.direction = this->LowerLeftCorner + u*this->Horizontal + v*this->Vertical - this->Origin;
        return Ray;
    }
};

inline vec3 RayColor(ray& Ray, world& World)
{   
    hit_record HitRecord;
    if(World.hit(Ray, HitRecord, 0.0f, Inf))
    {
        return 0.5f * (HitRecord.normal + vec(1.0f, 1.0f, 1.0f));
    }
    vec3 UnitDirection = normalize(Ray.direction);
    real32 t = (real32)0.5f*(UnitDirection.y + 1.0f);
    vec3 color1 = vec(1.0f, 1.0f, 1.0f);
    vec3 color2 = vec(0.5f, 0.7f, 1.0f);
    return lerp(color1, color2, t);
}