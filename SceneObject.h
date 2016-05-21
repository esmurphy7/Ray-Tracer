//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_SCENEOBJECT_H
#define CSC305_A1_SCENEOBJECT_H


#include "Vec3.h"
#include "RGB_Color.h"
#include "Ray.h"

struct HitRecord
{
    float t;
    Vec3f point;
    Vec3f normal;
};

class SceneObject {
public:
    Vec3f center;
    RGB_Color surfaceColor;
    float emission;
    SceneObject();
    SceneObject(Vec3f center, RGB_Color surfaceColor, float emission);
    virtual ~SceneObject() {}
    virtual bool intersects(float t_min, float t_max, Ray ray, HitRecord& hitRecord) = 0;
};

SceneObject::SceneObject()
        : center(Vec3f(0.0f, 0.0f, 0.0f)),
          surfaceColor(RGB_Color(0.0f, 0.0f, 0.0f)),
          emission(0.0f)
{}

SceneObject::SceneObject(Vec3f center, RGB_Color surfaceColor, float emission)
{
    this->center=center;
    this->surfaceColor=surfaceColor;
    this->emission=emission;
}

#endif //CSC305_A1_SCENEOBJECT_H
