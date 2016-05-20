//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_SCENE_H
#define CSC305_A1_SCENE_H

#include <vector>
#include <iomanip>
#include <GL/gl.h>
#include <limits>
#include "SceneObject.h"
#include "RGB_Color.h"
#include "Ray.h"

#define ANTI_ALIASING_SAMPLES 100

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
    RGB_Color calculateColor(Ray r);
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
            // anti-aliasing: send multiple rays to the current pixel and take average color
            RGB_Color color;
            for(int s=0; s < ANTI_ALIASING_SAMPLES; s++)
            {
                // calculate random sampling scalars used to scale directional vectors
                float rv = ((float) rand() / (RAND_MAX));
                float ru = ((float) rand() / (RAND_MAX));
                float v = (i + rv) / float(pixelmap.size());
                float u = (j + ru) / float(pixelmap[i].size());

                // create ray at position of camera
                // use scaled directional vectors to define ray's direction
                Ray ray = Ray(camera.position, top_left_corner + horizontal*u + vertical*v - camera.position);

                // get the color and contribute it to average
                RGB_Color colorSample = calculateColor(ray);
                color.R += colorSample.R;
                color.G += colorSample.G;
                color.B += colorSample.B;
            }

            // take the average color
            Vec3f colorVec = color.toVec3f();
            colorVec /= ANTI_ALIASING_SAMPLES;
            color = RGB_Color::toColor(colorVec);
            pixelmap[i][j] = color;
        }
    }
}

/*
 * Calculate the color that the ray will produce based on its intersection of scene objects
 */
RGB_Color Scene::calculateColor(Ray ray)
{
    // default blended color
    Vec3f ray_unit_direction = Vec3f::unitVector(ray.direction);
    float t = 0.5f*(ray_unit_direction.getY() + 1.0f);
    Vec3f colorVec = Vec3f(1.0f, 1.0f, 1.0f)*(1.0f-t) + Vec3f(0.5f, 0.7f, 1.0f)*t;
    RGB_Color color = RGB_Color(colorVec.getX(), colorVec.getY(), colorVec.getZ());

    // find the closest object that the ray will intersect
    float t_max = std::numeric_limits<float>::max();    // only consider values of t less than max
    float t_min = 0.0f;                                 // only consider values of t greater than min
    HitRecord hitRecord;                                // information on any hits that occurred
    int hitIndex = -1;                                  // index of closest object hit
    float closest_t = t_max;                            // t value of closest hit point
    for(int o=0; o<objects.size(); o++)
    {
        // find intersection point that is closer than the closest so far
        bool hit = objects[o]->intersects(t_min, closest_t, ray, hitRecord);
        if(hit)
        {
            // record closest t value and index of object hit
            closest_t = hitRecord.t;
            hitIndex = o;
        }
    }

    // color the pixel if it was hit
    if(hitIndex != -1)
    {
        // get random point in a unit sphere
        Vec3f s;
        do {
            float rx = ((float) rand() / (RAND_MAX));
            float ry = ((float) rand() / (RAND_MAX));
            float rz = ((float) rand() / (RAND_MAX));
            s = Vec3f(rx, ry, rz)*2.0f - Vec3f(1.0f, 1.0f, 1.0f);
        } while(Vec3f::dotProduct(s, s) >= 1.0f);

        // get random point relative to normal of the hit point as target
        Vec3f target = hitRecord.point + hitRecord.normal + s;

        // create shadow ray
        Ray shadowRay = Ray(hitRecord.point, target - hitRecord.point);

        // recursively build color from shadow ray
        RGB_Color shadowColor = calculateColor(shadowRay);
        shadowColor.R *= 0.5f;
        shadowColor.G *= 0.5f;
        shadowColor.B *= 0.5f;

        return shadowColor;
    }

    return color;
}

#endif //CSC305_A1_SCENE_H

