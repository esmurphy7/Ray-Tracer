//
// Created by Evan on 5/14/2016.
//

#ifndef CSC305_A1_PLANE_H
#define CSC305_A1_PLANE_H

#include <cmath>
#include "Vec3.h"
#include "SceneObject.h"

class Plane: public SceneObject
{
public:
    Vec3f normal;
    Plane(Vec3f center,  Vec3f normal, RGB_Color surfaceColor, float emission);
    bool intersects(float t_min, float t_max, Ray ray, HitRecord& hitRecord);
};

Plane::Plane(Vec3f center, Vec3f normal, RGB_Color surfaceColor, float emission)
        : SceneObject(center, surfaceColor, emission)
{
    this->normal = normal;
}

bool Plane::intersects(float t_min, float t_max, Ray ray, HitRecord &hitRecord)
{
    float denom = Vec3f::dotProduct(normal, ray.direction);
    if (std::abs(denom) > 0.0001f)
    {
        float t = Vec3f::dotProduct((center - ray.origin), normal) / denom;
        if (t >= 0.0f)
        {
            hitRecord.t = t;
            hitRecord.point = ray.pointAt(t);
            hitRecord.normal = normal;
            return true;
        }
    }
    return false;
}
#endif //CSC305_A1_PLANE_H
