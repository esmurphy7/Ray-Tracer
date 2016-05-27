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
    RGB_Color(Vec3f vec);
    RGB_Color(float R, float G, float B);
    Vec3f toVec3f();
    static RGB_Color toColor(const Vec3f& vec);

    void operator += (const RGB_Color& color)
    {
        R += color.R;
        G += color.G;
        B += color.B;
    }

    RGB_Color operator * (const RGB_Color& color) const
    {
        return RGB_Color(R*color.R, G*color.G, B*color.B);
    }

    RGB_Color operator * (const float& val) const
    {
        return RGB_Color(R*val, G*val, B*val);
    }
};

RGB_Color::RGB_Color()
{
    R, G, B = 0.0f;
}

RGB_Color::RGB_Color(Vec3f vec)
{
    R = vec.getX();
    G = vec.getY();
    B = vec.getZ();
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
