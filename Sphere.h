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
    Sphere();
    Sphere(Vect3 center, RGB_Color surfaceColor, double emission, double radius);
    double radius;
};

Sphere::Sphere()
        : SceneObject()
{
    radius = 0;
}

Sphere::Sphere(Vect3 center, RGB_Color surfaceColor, double emission, double radius)
        : SceneObject(center, surfaceColor, emission)
{
    this->radius = radius;
}

#endif //CSC305_A1_SPHERE_H
