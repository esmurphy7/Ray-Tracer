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
    float intersects(Ray ray);
};

Sphere::Sphere()
        : SceneObject(),
          radius(0.0f)
{}

Sphere::Sphere(Vec3f center, RGB_Color surfaceColor, float emission, float radius)
        : SceneObject(center, surfaceColor, emission),
          radius(radius)
{}

float Sphere::intersects(Ray ray)
{
    Vec3f center_offset = ray.origin - center;
    float a = Vec3f::dotProduct(ray.direction, ray.direction);
    float b = 2.0f * Vec3f::dotProduct(center_offset, ray.direction);
    float c = Vec3f::dotProduct(center_offset, center_offset) - radius*radius;

    float discriminant = b*b - 4.0f*a*c;

    // no solution
    if(discriminant < 0.0f)
    {
        return -1.0f;
    }
    // 1 or 2 solutions
    else
    {
        float t = float((-b - sqrt(discriminant)) / (2.0f*a));
        return t;
    }
}

#endif //CSC305_A1_SPHERE_H
