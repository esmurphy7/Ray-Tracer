//
// Created by Evan on 5/14/2016.
//

#ifndef CSC305_A1_RGB_COLOR_H
#define CSC305_A1_RGB_COLOR_H

class RGB_Color
{
public:
    float R, G, B;
    RGB_Color();
    RGB_Color(float R, float G, float B);
    Vec3f toVec3f();
    static RGB_Color toColor(const Vec3f& vec);
};

RGB_Color::RGB_Color()
{
    R, G, B = 0.0f;
}

RGB_Color::RGB_Color(float R, float G, float B)
{
    this->R = R;
    this->G = G;
    this->B = B;
}

Vec3f RGB_Color::toVec3f()
{
    Vec3f vec = Vec3f(R, G, B);
    return vec;
}

RGB_Color RGB_Color::toColor(const Vec3f &vec)
{
    RGB_Color color = RGB_Color(vec.getX(), vec.getY(), vec.getZ());
    return color;
}
#endif //CSC305_A1_RGB_COLOR_H
