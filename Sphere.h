//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_SPHERE_H
#define CSC305_A1_SPHERE_H

#include "Vect3.h"
#include "SceneObject.h"

class Sphere: public SceneObject
{
public:
    float radius;
    Sphere();
    Sphere(Vect3 center, RGB_Color surfaceColor, float emission, float radius);
    Vect3 intersects(Ray ray);
};

Sphere::Sphere()
        : SceneObject(),
          radius(0)
{}

Sphere::Sphere(Vect3 center, RGB_Color surfaceColor, float emission, float radius)
        : SceneObject(center, surfaceColor, emission),
          radius(radius)
{}

Vect3 Sphere::intersects(Ray ray)
{
    float a = ray.direction.dot(ray.direction);
    float b = ray.direction.dot(ray.origin);
    float c = ray.origin.dot(ray.origin) - 1.0f;

    float t0 = -b + sqrt((b*b) - a*c) / a;
    float t1 = -b + sqrt((b*b) - a*c) / a;

    Vect3 hit0 = ray.origin.subtract(ray.direction*t0);
    Vect3 hit1 = ray.origin.subtract(ray.direction*t1);

    int bp=0;
}

#endif //CSC305_A1_SPHERE_H
