//
// Created by Evan on 5/11/2016.
//

#ifndef CSC305_A1_SCENE_H
#define CSC305_A1_SCENE_H

#include <vector>
#include <iomanip>
#include <limits>
#include <stdlib.h>
#include "SceneObject.h"
#include "RGB_Color.h"
#include "Ray.h"
#include "Sphere.h"

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
    void addObject(SceneObject* object);
    void traceRays();

private:
    Camera camera;
    ImagePlane imgPlane;
    std::vector<SceneObject*> lights;
    RGB_Color calculateColor(Ray r);
    int findClosestIntersecting(Ray r, std::vector<SceneObject*> objectSet, HitRecord& hitRecord);
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

    // initialize light set
    lights = std::vector<SceneObject*>();

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

void Scene::addObject(SceneObject *object)
{
    if(object->emission > 0.0f)
    {
        lights.push_back(object);
    }
    else
    {
        objects.push_back(object);
    }
}

void Scene::traceRays()
{
    // store the position of the top-left corner of the image plane
    Vec3f bottom_left_corner = Vec3f(imgPlane.position.getX() - imgPlane.width*0.5f,
                                  imgPlane.position.getY() - imgPlane.height*0.5f,
                                  imgPlane.position.getZ());

    // directional vectors scaled by the dimensions of the image plane
    Vec3f horizontal = Vec3f(float(imgPlane.width), 0.0f, 0.0f);
    Vec3f vertical = Vec3f(0.0f, float(imgPlane.height), 0.0f);

    for(int i=0; i<pixelmap.size(); i++)
    {
        for(int j=0; j<pixelmap[i].size(); j++)
        {
            /*
            // anti-aliasing: send multiple rays to the current pixel and take average color
            RGB_Color color = RGB_Color(0.0f,0.0f,0.0f);
            for(int s=0; s < ANTI_ALIASING_SAMPLES; s++)
            {
                // calculate random sampling scalars used to scale directional vectors
                float rv = ((float) rand() / (RAND_MAX));
                float ru = ((float) rand() / (RAND_MAX));
                float v = (i + rv) / float(pixelmap.size());
                float u = (j + ru) / float(pixelmap[i].size());

                // create ray at position of camera
                // use scaled directional vectors to define ray's direction
                Ray ray = Ray(camera.position, bottom_left_corner + horizontal*u + vertical*v - camera.position);

                // get the color and contribute it to average
                int debug_recurse_i = 0;
                RGB_Color colorSample = calculateColor(ray, debug_recurse_i);
                color.R += colorSample.R;
                color.G += colorSample.G;
                color.B += colorSample.B;
            }

            // take the average color
            Vec3f colorVec = color.toVec3f();
            colorVec /= ANTI_ALIASING_SAMPLES;
            color = RGB_Color(float(sqrt(colorVec.getX())), float(sqrt(colorVec.getY())), float(sqrt(colorVec.getZ())));
             */

            // calculate random sampling scalars used to scale directional vectors
            float v = (i) / float(pixelmap.size());
            float u = (j) / float(pixelmap[i].size());

            // create ray at position of camera
            // use scaled directional vectors to define ray's direction
            Ray ray = Ray(camera.position, bottom_left_corner + horizontal*u + vertical*v - camera.position);
            RGB_Color color = calculateColor(ray);
            pixelmap[i][j] = color;

        }
    }
}

/*
 * Calculate the color that the ray will produce based on its intersection of scene objects
 */
RGB_Color Scene::calculateColor(Ray ray)
{
    // if the ray intersected an object compute lambertian color, otherwise black
    HitRecord hitRec;
    int closestIndex = findClosestIntersecting(ray, objects, hitRec);
    if(closestIndex != -1)
    {
        Sphere light_sphere = Sphere(Vec3f(-4.0f,2.0f,-4.0f), RGB_Color(1.0f,1.0f,1.0f), 2.0f, 0.5f);
        Vec3f light_direction = Vec3f::unitVector(light_sphere.center - ray.pointAt(hitRec.t));

        // ratio of deflected light
        float objectAlbedo = 1.0f;

        Vec3f illumination = light_sphere.surfaceColor.toVec3f();              // illumination
        float diffCoefficient = (objectAlbedo / 3.141592654f);                            // diffuse coefficient
        float lightIntensity = light_sphere.emission;                                     // intensity
        Vec3f objectColor = objects[closestIndex]->surfaceColor.toVec3f(); // object color
        float normalDotLight = std::max(0.0f, hitRec.normal.dotProduct(light_direction));  // proportional to angle of incident

        Vec3f hitColor = illumination;
        hitColor *= diffCoefficient;
        hitColor *= lightIntensity;
        hitColor *= normalDotLight;

        hitColor.setX(hitColor.getX() * objectColor.getX());
        hitColor.setY(hitColor.getY() * objectColor.getY());
        hitColor.setZ(hitColor.getZ() * objectColor.getZ());

        return RGB_Color::toColor(hitColor);

    }
    else
    {
        return RGB_Color(0.0f,0.0f,0.0f);
    }

    /*
    // if the ray hit a light directly
    HitRecord lightHitRecord;
    int closestLightIndex = findClosestIntersecting(ray, lights, lightHitRecord);
    if(closestLightIndex != -1)
    {
        RGB_Color lightColor = lights[closestLightIndex]->surfaceColor;
        return lightColor;
    }

    // find the closest object that the ray will intersect
    HitRecord objectHitRecord;
    int closestObjectIndex = findClosestIntersecting(ray, objects, objectHitRecord);
    // if the ray hit a diffuse object, send reflection ray and get its color
    if(closestObjectIndex != -1)
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
        Vec3f target = objectHitRecord.point + objectHitRecord.normal + s;

        // create reflect ray
        Ray reflectRay = Ray(objectHitRecord.point, target - objectHitRecord.point);

        // recursively calculate the color of the reflection ray
        RGB_Color aggregatedColor = RGB_Color(0.0f,0.0f,0.0f);
        for(int l=0; l<lights.size(); l++)
        {
            Vec3f lightDirection = lights[l]->center - objectHitRecord.point;
            lightDirection.normalize();
            aggregatedColor += objects[closestObjectIndex]->surfaceColor *
                               std::max(float(0), Vec3f::dotProduct(objectHitRecord.normal, lightDirection)) * lights[l]->surfaceColor;
        }

        return aggregatedColor;
    }

    // ray didn't intersect with anything
    RGB_Color defaultColor = RGB_Color(0.0f,0.0f,0.0f);
    return defaultColor;
     */
}

int Scene::findClosestIntersecting(Ray ray, std::vector<SceneObject *> objectSet, HitRecord& hitRecord)
{
    float t_max = std::numeric_limits<float>::max();    // only consider values of t less than max
    float t_min = 0.0f;                                 // only consider values of t greater than min
    int closestIndex = -1;               // closest object hit
    float closest_t = t_max;                            // t value of closest hit point
    for(int o=0; o<objectSet.size(); o++)
    {
        // find intersection point that is closer than the closest so far
        bool hit = objectSet[o]->intersects(t_min, closest_t, ray, hitRecord);
        if(hit)
        {
            // record closest t value and index of object hit
            closest_t = hitRecord.t;
            closestIndex = o;
        }
    }

    return closestIndex;
}
#endif //CSC305_A1_SCENE_H

