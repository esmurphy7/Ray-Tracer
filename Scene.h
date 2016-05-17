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
    std::vector<SceneObject*> objects;
    Scene(unsigned int imagePlanew, unsigned int imagePlaneh, double focalLength);
    void traceRays();

private:
    ImagePlane imgPlane;
    Camera camera = Camera(Vect3(0.0f, 0.0f, -10.0f));
};

Scene::Scene(unsigned int imagePlanew, unsigned int imagePlaneh, double focalLength)
{
    // create the image plane at the origin
    imgPlane = {
            Vect3(0.0f, 0.0f, 0.0f),   // position
            Vect3(0.0f, 0.0f, 1.0f),   // normal
            imagePlanew,    // width
            imagePlaneh     // height
    };

    // initialize the camera at the position specified by the focal length
    Vect3 pos = imgPlane.position.subtract(imgPlane.normal*focalLength);
    //camera = Camera(pos);

    // initialize object set
    objects = std::vector<SceneObject*>();

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

Vect3 VectorSubtract(Vect3 v1, Vect3 v2)
{
    Vect3 result = Vect3();
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

void Scene::traceRays()
{
    // for each pixel on the image plane, generate a ray with its origin at the camera
    for(int i=0; i<pixelmap.size(); i++)
    {
        for(int j=0; j<pixelmap[i].size(); j++)
        {
            float fi = (float)i;
            float fj = (float)j;
            float imgZ = imgPlane.position.z;
            Vect3 pixelPos = Vect3(fj, fi, imgZ);
            Vect3 rayDirection = VectorSubtract(pixelPos, camera.position);
            Ray ray = Ray(camera.position, rayDirection);

            int closest_t = 0;
            /*for(int o=0; o<objects.size(); o++)
            {
                Vect3 hitPoint = objects[o]->intersects(ray);

            }
             */
        }
    }
}

#endif //CSC305_A1_SCENE_H

