//
// Created by Evan on 5/14/2016.
//

#ifndef CSC305_A1_PLANE_H
#define CSC305_A1_PLANE_H

#include "Vec3.h"
#include "SceneObject.h"

class Plane: SceneObject
{
public:
    Vec3f normal;
    float width, height;
    Plane(Vec3f norm, float w, float h);
    Plane(Vec3f center, RGB_Color surfaceColor, float emission, Vec3f normal, float width,
          float height);
};

Plane::Plane(Vec3 norm, float w, float h)
        : SceneObject()
{
    normal = norm;
    width = w;
    height = h;
}

Plane::Plane(Vec3f center, RGB_Color surfaceColor, float emission, Vec3f normal, float width, float height)
        : SceneObject(center, surfaceColor, emission)
{
    this->normal = normal;
    this->width = width;
    this->height = height;
}

#endif //CSC305_A1_PLANE_H
