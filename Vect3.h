//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_VECT_H
#define CSC305_A1_VECT_H


#include <math.h>

class Vect3 {
public:
    float x;
    float y;
    float z;

    Vect3();
    Vect3(float x, float y, float z);

    // vector operator overloads
    Vect3 operator * (const float &f) const { return Vect3(x*f, y*f, z*f); }
    Vect3 operator * (const Vect3 &v) const { return Vect3(x*v.x, y*v.y, z*v.z); }
    float dot(const Vect3 &v) const { return x*v.x + y*v.y + z*v.z; }
    Vect3 cross(const Vect3 &v)
    {
        return Vect3(y * v.z - z * v.y,
                     z * v.x - x * v.z,
                     x * v.y - y * v.x);
    }
    Vect3 subtract(const Vect3 &v)
    {
        Vect3 ret = Vect3(x - v.x, y - v.y, z - v.z);
        return ret;
    }
    Vect3 operator + (const Vect3 &v) const { return Vect3(x + v.x, y + v.y, z + v.z); }
    Vect3& operator += (const Vect3 &v) { x += v.x, y += v.y, z += v.z; return *this; }
    Vect3& operator *= (const Vect3 &v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
    //Vect3 operator - () const { return Vect3(-x, -y, -z); }
    float magnitude_sqrd() const { return x * x + y * y + z * z; }
    float magnitude() const { return sqrt(magnitude_sqrd()); }
    Vect3 normalize() const
    {
        float mag = magnitude();
        return Vect3(x/mag, y/mag, z/mag);
    }

};

Vect3::Vect3()
    : x(0.0f),
      y(0.0f),
      z(0.0f)
{}

Vect3::Vect3(float xc, float yc, float zc)
    : x(xc),
      y(yc),
      z(zc)
{}

#endif //CSC305_A1_VECT_H
