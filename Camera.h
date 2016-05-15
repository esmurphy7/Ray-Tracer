//
// Created by Evan on 5/14/2016.
//

#ifndef CSC305_A1_CAMERA_H
#define CSC305_A1_CAMERA_H

#include "Vect3.h"

class Camera
{
public:
    Vect3 position;
    Vect3 direction;
    Vect3 up;
    Vect3 right;
    Camera();
    Camera(Vect3 pos);
};

Camera::Camera()
{
    position = Vect3(0, 0, 0);
    direction   = Vect3(0, 0, 1);
    up          = Vect3(0, 1, 0);
    right       = Vect3(1, 0, 0);
}

Camera::Camera(Vect3 pos)
{
    position = pos;
    direction   = Vect3(0, 0, 1);
    up          = Vect3(0, 1, 0);
    right       = Vect3(1, 0, 0);
}

#endif //CSC305_A1_CAMERA_H
