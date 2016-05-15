//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_VECT_H
#define CSC305_A1_VECT_H


#include <math.h>

class Vect3 {
public:
    double x;
    double y;
    double z;

    Vect3();
    Vect3(double x, double y, double z);

    // vector operator overloads
    Vect3 operator * (const double &f) const { return Vect3(x*f, y*f, z*f); }
    Vect3 operator * (const Vect3 &v) const { return Vect3(x*v.x, y*v.y, z*v.z); }
    double dot(const Vect3 &v) const { return x*v.x + y*v.y + z*v.z; }
    Vect3 cross(const Vect3 &v) {
        return Vect3(y * v.z - z * v.y,
                     z * v.x - x * v.z,
                     x * v.y - y * v.x);
    }
    Vect3 operator - (const Vect3 &v) const { return Vect3(x - v.x, y - v.y, z - v.z); }
    Vect3 operator + (const Vect3 &v) const { return Vect3(x + v.x, y + v.y, z + v.z); }
    Vect3& operator += (const Vect3 &v) { x += v.x, y += v.y, z += v.z; return *this; }
    Vect3& operator *= (const Vect3 &v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
    //Vect3 operator - () const { return Vect3(-x, -y, -z); }
    double magnitude_sqrd() const { return x * x + y * y + z * z; }
    double magnitude() const { return sqrt(magnitude_sqrd()); }
    Vect3 normalize() const
    {
        double mag = magnitude();
        return Vect3(x/mag, y/mag, z/mag);
    }

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

#endif //CSC305_A1_VECT_H
