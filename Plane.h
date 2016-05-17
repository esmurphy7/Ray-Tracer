//
// Created by Evan on 5/14/2016.
//

#ifndef CSC305_A1_PLANE_H
#define CSC305_A1_PLANE_H

#include "Vect3.h"
#include "SceneObject.h"

class Plane: SceneObject
{
public:
    Vect3 normal;
    float width, height;
    Plane(Vect3 norm, float w, float h);
    Plane(Vect3 center, RGB_Color surfaceColor, float emission, Vect3 normal, float width,
          float height);
};

Plane::Plane(Vect3 norm, float w, float h)
        : SceneObject()
{
    normal = norm;
    width = w;
    height = h;
}

Plane::Plane(Vect3 center, RGB_Color surfaceColor, float emission, Vect3 normal, float width, float height)
        : SceneObject(center, surfaceColor, emission)
{
    this->normal = normal;
    this->width = width;
    this->height = height;
}

#endif //CSC305_A1_PLANE_H
