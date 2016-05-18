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
};

Ray::Ray()
: origin(Vec3f()), direction(Vec3f()) { }

Ray::Ray(Vec3f origin, Vec3f direction)
{
    this->origin=origin;
    this->direction=direction;
}

#endif //CSC305_A1_RAY_H
