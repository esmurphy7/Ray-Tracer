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
};

RGB_Color::RGB_Color()
{
    R, G, B = 0;
}

RGB_Color::RGB_Color(float R, float G, float B)
{
    this->R = R;
    this->G = G;
    this->B = B;
}

#endif //CSC305_A1_RGB_COLOR_H
