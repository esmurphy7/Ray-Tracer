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
    Vec3f intersects(Ray ray);
};

Sphere::Sphere()
        : SceneObject(),
          radius(0.0f)
{}

Sphere::Sphere(Vec3f center, RGB_Color surfaceColor, float emission, float radius)
        : SceneObject(center, surfaceColor, emission),
          radius(radius)
{}

Vec3f Sphere::intersects(Ray ray)
{
    Vec3f s = ray.origin;
    Vec3f d = ray.direction;
    Vec3f c = center;
    float r = radius;

    // Calculate ray start's offset from the sphere center
    Vec3f p = s - c;

    float rSquared = r * r;
    float p_d = Vec3f::dotProduct(p, d);

    // The sphere is behind or surrounding the start point.
    if(p_d < 0 || Vec3f::dotProduct(p, p) < rSquared)
        return Vec3f();

    // Flatten p into the plane passing through c perpendicular to the ray.
    // This gives the closest approach of the ray to the center.
    Vec3f a = p - d*p_d;

    float aSquared = Vec3f::dotProduct(a, a);

    // Closest approach is outside the sphere.
    if(aSquared > rSquared)
        return Vec3f();

    // Calculate distance from plane where ray enters/exits the sphere.
    float h = sqrtf(rSquared - aSquared);

    // Calculate intersection point relative to sphere center.
    Vec3f i = a - d*h;

    Vec3f intersection = c + i;
    Vec3f normal = i/r;
    // We've taken a shortcut here to avoid a second square root.
    // Note numerical errors can make the normal have length slightly different from 1.
    // If you need higher precision, you may need to perform a conventional normalization.

    intersection.print("intersection");
    return intersection, normal;

}

#endif //CSC305_A1_SPHERE_H
