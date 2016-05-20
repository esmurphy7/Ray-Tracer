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
#include "Ray.h"

const RGB_Color backgroundColor = RGB_Color(0.0f, 0.0f, 0.0f);

struct ImagePlane
{
    Vec3f position;
    Vec3f normal;
    unsigned int width;
    unsigned int height;
};

struct Camera
{
    Vec3f position;
    Vec3f forward;
    Vec3f up;
    Vec3f right;
};

class Scene
{
public:
    std::vector<std::vector<RGB_Color>> pixelmap;
    std::vector<SceneObject*> objects;
    Scene(unsigned int width, unsigned int height, double focalLength);
    void traceRays();

private:
    Camera camera;
    ImagePlane imgPlane;
};

Scene::Scene(unsigned int width, unsigned int height, double focalLength)
{
    // initialize camera
    camera = {
            Vec3f(0.0f, 0.0f, 0.0f),    // position
            Vec3f(0.0f, 0.0f, -1.0f),   // forward
            Vec3f(0.0f, 1.0f, 0.0f),    // up
            Vec3f(1.0f, 0.0f, 0.0f),    // right
    };

    // initialize image plane at position specified by focal length
    Vec3f pos = camera.position + Vec3f(0.0f, 0.0f, -1.0f)*focalLength;
    imgPlane = {
            pos,                        // position
            Vec3f(0.0f, 0.0f, -1.0f),   // normal
            4,                          // width
            2                           // height
    };

    // initialize object set
    objects = std::vector<SceneObject*>();

    // initialize pixel map
    pixelmap = std::vector<std::vector<RGB_Color>>(height);
    for(int i=0; i < height; i++)
    {
        pixelmap[i] = std::vector<RGB_Color>(width);

        //initialize pixelmap to background color
        for(int j=0; j < width; j++)
        {
            pixelmap[i][j] = backgroundColor;
        }
    }
}

void Scene::traceRays()
{
    // store the position of the top-left corner of the image plane
    Vec3f top_left_corner = Vec3f(imgPlane.position.getX() - imgPlane.width*0.5f,
                                  imgPlane.position.getY() - imgPlane.height*0.5f,
                                  imgPlane.position.getZ());

    // directional vectors scaled by the dimensions of the image plane
    Vec3f horizontal = Vec3f(float(imgPlane.width), 0.0f, 0.0f);
    Vec3f vertical = Vec3f(0.0f, float(imgPlane.height), 0.0f);

    for(int i=0; i<pixelmap.size(); i++)
    {
        for(int j=0; j<pixelmap[i].size(); j++)
        {
            // calculate scalars used to scale directional vectors
            float v = float(i) / float(pixelmap.size());
            float u = float(j) / float(pixelmap[i].size());

            // create ray at position of camera
            // use scaled directional vectors to define ray's direction
            Ray ray = Ray(camera.position, top_left_corner + horizontal*u + vertical*v);

            // compute blended color
            Vec3f ray_unit_direction = Vec3f::unitVector(ray.direction);
            float t = 0.5f*(ray_unit_direction.getY() + 1.0f);
            Vec3f colorVec = Vec3f(1.0f, 1.0f, 1.0f)*(1.0f-t) + Vec3f(0.5f, 0.7f, 1.0f)*t;
            RGB_Color color = RGB_Color(colorVec.getX(), colorVec.getY(), colorVec.getZ());

            // check if the ray has intersected an object in the scene
            for(int o=0; o<objects.size(); o++)
            {
                t = objects[o]->intersects(ray);
                if(t > 0.0f)
                {
                    Vec3f objectNormal = ray.pointAt(t) - objects[o]->center;
                    objectNormal.normalize();

                    colorVec = Vec3f(objectNormal.getX()+1, objectNormal.getY()+1, objectNormal.getZ()+1);
                    colorVec = colorVec*0.5f;

                    color = RGB_Color(colorVec.getX(), colorVec.getY(), colorVec.getZ());
                    //color = objects[o]->surfaceColor;
                    break;
                }
            }

            pixelmap[i][j] = color;
        }
    }
}

#endif //CSC305_A1_SCENE_H

