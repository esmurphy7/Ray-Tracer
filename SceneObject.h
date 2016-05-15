//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_SCENEOBJECT_H
#define CSC305_A1_SCENEOBJECT_H


#include "Vect3.h"
#include "RGB_Color.h"

class SceneObject {
public:
    SceneObject();
    SceneObject(Vect3 center, RGB_Color surfaceColor, double emission);
    Vect3 center;
    RGB_Color surfaceColor;
    double emission;
};

SceneObject::SceneObject()
        : surfaceColor(RGB_Color(0.0, 0.0, 0.0))
{
    this->center = Vect3(0,0,0);
    this->emission = 0;
}

SceneObject::SceneObject(Vect3 center, RGB_Color surfaceColor, double emission)
        : surfaceColor(surfaceColor)
{
    this->center = center;
    this->surfaceColor = surfaceColor;
    this->emission = emission;
}

#endif //CSC305_A1_SCENEOBJECT_H
