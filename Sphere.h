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

private:
    bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);
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
    float t0, t1; // solutions for t if the ray intersects

    /*
    // geometric solution
    Vec3f L = center - ray.origin;
    float tca = L.dotProduct(ray.direction);

    if (tca < 0) return false;

    float d2 = L.dotProduct(L) - tca * tca;
    float radius2 = radius*radius;

    if (d2 > radius2) return false;

    float thc = float(sqrt(radius2 - d2));
    t0 = tca - thc;
    t1 = tca + thc;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0) return false; // both t0 and t1 are negative
    }
    */

    // analytic solution
    Vec3f L = ray.origin - center;
    float a = ray.direction.dotProduct(ray.direction);
    float b = 2 * ray.direction.dotProduct(L);
    float c = L.dotProduct(L) - radius*radius;
    if (!solveQuadratic(a, b, c, t0, t1)) return false;


    float t = t0;

    if(t_min > t || t > t_max)
    {
        return false;
    }

    hitRecord.t = t;
    hitRecord.point = ray.pointAt(t);

    Vec3f Phit = ray.origin + ray.direction * t;
    Vec3f Nhit = Phit - center;
    Nhit.normalize();

    hitRecord.normal = Nhit;

    return true;
}

bool Sphere::solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = - 0.5 * b / a;
    else {
        float q = (b > 0) ?
                  -0.5 * (b + sqrt(discr)) :
                  -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);

    return true;
}

#endif //CSC305_A1_SPHERE_H
