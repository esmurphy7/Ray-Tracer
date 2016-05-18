//
// Created by Evan on 5/14/2016.
//

#ifndef CSC305_A1_CAMERA_H
#define CSC305_A1_CAMERA_H

#include "Vec3.h"

class Camera
{
public:
    Vec3f position;
    Vec3f direction;
    Vec3f up;
    Vec3f right;
    Camera();
    Camera(Vec3f pos);
};

Camera::Camera()
    : position(Vec3f(0.0f, 0.0f, 0.0f)),
      direction(),
      up(Vec3f(0.0f, 1.0f, 0.0f)),
      right(Vec3f(1.0f, 0.0f, 0.0f))
{}

Camera::Camera(Vec3f pos)
{
    position = pos;
    direction = Vec3f(0.0f, 0.0f, 1.0f);
    up = Vec3f(0.0f, 1.0f, 0.0f);
    right = Vec3f(1.0f, 0.0f, 0.0f);
}

#endif //CSC305_A1_CAMERA_H
