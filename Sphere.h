//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_SPHERE_H
#define CSC305_A1_SPHERE_H

#include "Vec3.h"
#include "SceneObject.h"

class Sphere: public SceneObject
{
public:
    float radius;
    Sphere();
    Sphere(Vec3f center, RGB_Color surfaceColor, float emission, float radius);
    bool intersects(float t_min, float t_max, Ray ray, HitRecord& hitRecord);
};

Sphere::Sphere()
        : SceneObject(),
          radius(0.0f)
{}

Sphere::Sphere(Vec3f center, RGB_Color surfaceColor, float emission, float radius)
        : SceneObject(center, surfaceColor, emission),
          radius(radius)
{}

bool Sphere::intersects(float t_min, float t_max, Ray ray, HitRecord& hitRecord)
{
    Vec3f center_offset = ray.origin - center;
    float a = Vec3f::dotProduct(ray.direction, ray.direction);
    float b = 2.0f * Vec3f::dotProduct(center_offset, ray.direction);
    float c = Vec3f::dotProduct(center_offset, center_offset) - radius*radius;

    float discriminant = b*b - 4.0f*a*c;

    // no solution
    if(discriminant < 0.0f)
    {
        return false;
    }
    // 1 or 2 solutions
    else
    {
        // check if negative discriminant is within limits
        //float t = float((-b - sqrt(discriminant)) / 2.0f*a);
        float t = float((-b - sqrt(b*b - a*c)/a));
        if(t_min < t && t < t_max)
        {
            hitRecord.t = t;
            hitRecord.point = ray.pointAt(t);
            hitRecord.normal = (hitRecord.point - center) / radius;
            return true;
        }

        // check if positive discriminant is within limits
        //t = float((-b + sqrt(discriminant)) / 2.0f*a);
        t = float((-b + sqrt(b*b - a*c)/a));
        if(t_min < t && t < t_max)
        {
            hitRecord.t = t;
            hitRecord.point = ray.pointAt(t);
            hitRecord.normal = (hitRecord.point - center) / radius;
            return true;
        }
        return false;
    }
}

#endif //CSC305_A1_SPHERE_H
