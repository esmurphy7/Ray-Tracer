//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_SCENE_H
#define CSC305_A1_SCENE_H

#include <vector>
#include "SceneObject.h"
#include "RGB_Color.h"
#include "Camera.h"
#include "Ray.h"

struct ImagePlane
{
    Vect3 position;
    Vect3 normal;
    unsigned int width;
    unsigned int height;
};

class Scene
{
public:
    std::vector<std::vector<RGB_Color>> pixelmap;
    std::vector<SceneObject> objects;
    Scene(unsigned int imagePlanew, unsigned int imagePlaneh, double focalLength);
    void generateRays();

private:
    ImagePlane imgPlane;
    Camera camera;
};

Scene::Scene(unsigned int imagePlanew, unsigned int imagePlaneh, double focalLength)
{
    // create the image plane at the origin
    imgPlane = {
            Vect3(0,0,0),   // position
            Vect3(0,0,1),   // normal
            imagePlanew,    // width
            imagePlaneh     // height
    };

    // initialize the camera at the position specified by the focal length
    Vect3 pos = imgPlane.position - imgPlane.normal*focalLength;
    camera = Camera(pos);

    // initialize object set
    objects = std::vector<SceneObject>();

    // initialize pixel map
    pixelmap = std::vector<std::vector<RGB_Color>>(imagePlaneh);
    for(int i=0; i < imagePlaneh; i++)
    {
        pixelmap[i] = std::vector<RGB_Color>(imagePlanew);

        //DEBUG: fill each cell with test rgb value
        for(int j=0; j < imagePlanew; j++)
        {
            pixelmap[i][j] = RGB_Color(0.0, 0.0, 0.0);
        }
    }
}

void Scene::generateRays()
{
    // for each pixel on the image plane, generate a ray with its origin at the camera
    for(int i=0; i<pixelmap.size(); i++)
    {
        for(int j=0; j<pixelmap[i].size(); j++)
        {
            Vect3 pixelPos = Vect3(j, i, imgPlane.position.z);
            Ray ray = Ray(camera.position, pixelPos - camera.position);
            int bp = 0;
        }
    }
}

#endif //CSC305_A1_SCENE_H

