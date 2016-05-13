//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_VECT_H
#define CSC305_A1_VECT_H


#include <math.h>

class Vect3 {
public:
    Vect3();
    Vect3(double x, double y, double z);
    double magnitude();
    Vect3 normalize();

private:
    double x;
    double y;
    double z;
};

Vect3::Vect3()
{
    x, y, z = 0;
}

Vect3::Vect3(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

double Vect3::magnitude()
{
    return sqrt((x * x) + (y * y) + (z * z));
}

Vect3 Vect3::normalize()
{
    double magn = magnitude();
    return Vect3(x / magn, y / magn, z / magn);
}

#endif //CSC305_A1_VECT_H
