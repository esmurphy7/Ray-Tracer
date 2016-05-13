//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_SCENE_H
#define CSC305_A1_SCENE_H

#include <vector>
#include "SceneObject.h"

class Scene
{
public:
    Scene(int width, int height);
    void addObject(SceneObject object);

private:
    double width;
    double height;
    double** pixels;
    std::vector<SceneObject> objects;
};

Scene::Scene(int width, int height)
{
    this->width = width;
    this->height = height;

    // initialize pixel map
    pixels = new double*[height];
    for(int i=0; i<height; i++)
    {
        pixels[i] = new double[width];
    }
}

void Scene::addObject(SceneObject object)
{
    objects.push_back(object);
}

#endif //CSC305_A1_SCENE_H

