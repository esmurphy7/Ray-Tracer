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
    : position(Vect3(0.0f, 0.0f, 0.0f)),
      direction(Vect3(0.0f, 0.0f, 1.0f)),
      up(Vect3(0.0f, 1.0f, 0.0f)),
      right(Vect3(1.0f, 0.0f, 0.0f))
{}

Camera::Camera(Vect3 pos)
    : position(pos),
      direction(Vect3(0.0f, 0.0f, 1.0f)),
      up(Vect3(0.0f, 1.0f, 0.0f)),
      right(Vect3(1.0f, 0.0f, 0.0f))
{}

#endif //CSC305_A1_CAMERA_H
