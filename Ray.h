//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_RAY_H
#define CSC305_A1_RAY_H

#include "Vect3.h"

class Ray
{
public:
    Vect3 origin;
    Vect3 direction;
    Ray();
    Ray(Vect3 origin, Vect3 direction);
};

Ray::Ray()
: origin(Vect3()), direction(Vect3()) { }

Ray::Ray(Vect3 origin, Vect3 direction)
    : origin(Vect3(origin)),
      direction(Vect3(direction))
{}

#endif //CSC305_A1_RAY_H
