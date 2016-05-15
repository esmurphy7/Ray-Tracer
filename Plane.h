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
    double width, height;
    Plane(Vect3 norm, double w, double h);
    Plane(Vect3 center, RGB_Color surfaceColor, double emission, Vect3 normal, double width,
              double height);
};

Plane::Plane(Vect3 norm, double w, double h)
        : SceneObject()
{
    normal = norm;
    width = w;
    height = h;
}

Plane::Plane(Vect3 center, RGB_Color surfaceColor, double emission, Vect3 normal, double width, double height)
        : SceneObject(center, surfaceColor, emission)
{
    this->normal = normal;
    this->width = width;
    this->height = height;
}

#endif //CSC305_A1_PLANE_H
