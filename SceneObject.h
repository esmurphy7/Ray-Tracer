//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_SCENEOBJECT_H
#define CSC305_A1_SCENEOBJECT_H


#include "Vect3.h"
#include "RGB_Color.h"
#include "Ray.h"

class SceneObject {
public:
    Vect3 center;
    RGB_Color surfaceColor;
    float emission;
    SceneObject();
    SceneObject(Vect3 center, RGB_Color surfaceColor, float emission);
    virtual ~SceneObject() {}
    virtual Vect3 intersects(Ray ray) = 0;
};

SceneObject::SceneObject()
        : center(Vect3(0.0f, 0.0f, 0.0f)),
          surfaceColor(RGB_Color(0.0, 0.0, 0.0)),
          emission(0)
{}

SceneObject::SceneObject(Vect3 center, RGB_Color surfaceColor, float emission)
        : center(center),
          surfaceColor(surfaceColor),
          emission(emission)
{}

#endif //CSC305_A1_SCENEOBJECT_H
