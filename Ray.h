//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_RAY_H
#define CSC305_A1_RAY_H

#include "Vec3.h"

class Ray
{
public:
    Vec3f origin;
    Vec3f direction;
    Ray();
    Ray(Vec3f origin, Vec3f direction);
    Vec3f pointAt(float t);
};

Ray::Ray()
: origin(Vec3f()), direction(Vec3f()) { }

Ray::Ray(Vec3f origin, Vec3f direction)
{
    this->origin=origin;
    this->direction=direction;
}

Vec3f Ray::pointAt(float t)
{
    Vec3f point = origin + direction*t;
    return point;
}

#endif //CSC305_A1_RAY_H
