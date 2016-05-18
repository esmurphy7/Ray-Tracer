//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_SCENE_H
#define CSC305_A1_SCENE_H

#include <vector>
#include <iomanip>
#include <GL/gl.h>
#include "SceneObject.h"
#include "RGB_Color.h"
#include "Camera.h"
#include "Ray.h"

struct ImagePlane
{
    Vec3f position;
    Vec3f normal;
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
    Camera camera;
    ImagePlane imgPlane;
};

Scene::Scene(unsigned int imagePlanew, unsigned int imagePlaneh, double focalLength)
{
    camera = Camera(Vec3f(0.0f, 0.0f, 0.0f));
    // create the image plane at the origin
    imgPlane = {
            Vec3f(0.0f, 0.0f, 10.0f),   // position
            Vec3f(0.0f, 0.0f, 1.0f),   // normal
            imagePlanew,    // width
            imagePlaneh     // height
    };

    // initialize the camera at the position specified by the focal length
    Vec3f pos = imgPlane.position - imgPlane.normal*focalLength;
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
            pixelmap[i][j] = RGB_Color(GL_ZERO, GL_ZERO, GL_ZERO);
        }
    }
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
            float imgZ = imgPlane.position.getZ();
            Vec3f pixelPos = Vec3f(fj, fi, imgZ);
            Vec3f rayDirection = pixelPos - camera.position;
            Vec3f rayOrigin = camera.position;
            Ray ray = Ray(rayOrigin, rayDirection);

            int closest_t = 0;
            for(int o=0; o<objects.size(); o++)
            {
                Vec3f hitPoint, normal = objects[o]->intersects(ray);

                int bp=0;
            }
        }
    }
}

#endif //CSC305_A1_SCENE_H

