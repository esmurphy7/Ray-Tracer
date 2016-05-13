//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_SCENEOBJECT_H
#define CSC305_A1_SCENEOBJECT_H


#include "Vect3.h"

class SceneObject {
public:
    SceneObject();
    SceneObject(Vect3 center, double surfaceColor, double emission);

private:
    Vect3 center;
    double surfaceColor;
    double emission;
};

SceneObject::SceneObject()
{
    center = Vect3(0, 0, 0);
    surfaceColor = 0;
    emission = 0;
}

SceneObject::SceneObject(Vect3 center, double surfaceColor, double emission)
{
    this->center = center;
    this->surfaceColor = surfaceColor;
    this->emission = emission;
}

#endif //CSC305_A1_SCENEOBJECT_H
