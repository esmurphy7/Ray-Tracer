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

#define ANTI_ALIASING_SAMPLES 50

const RGB_Color COLOR_BACKGROUND = RGB_Color(0.0f, 0.0f, 0.0f);

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
            pixelmap[i][j] = COLOR_BACKGROUND;
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
                RGB_Color colorSample = calculateColor(ray);
                color.R += colorSample.R;
                color.G += colorSample.G;
                color.B += colorSample.B;
            }

            // take the average color
            Vec3f colorVec = color.toVec3f();
            colorVec /= ANTI_ALIASING_SAMPLES;
            color = RGB_Color(colorVec);

            // set the color in the map
            pixelmap[i][j] = color;
        }
    }
}

/*
 * Calculate the color that the ray will produce based on its intersection of scene objects
 */
RGB_Color Scene::calculateColor(Ray ray)
{
    // iterate over all light sources
    Vec3f combinedColor = Vec3f(0.0f,0.0f,0.0f);
    for(int i=0; i<lights.size(); i++)
    {
        HitRecord hitRec;
        int closestIndex = findClosestIntersecting(ray, objects, hitRec);

        // if the ray didn't intersect an object, don't contribute a color
        if (closestIndex == -1)
        {
            // if the ray intersected a light directly
            HitRecord lightHitRec;
            if(lights[i]->intersects(0.0f, std::numeric_limits<float>::max(), ray, lightHitRec))
            {
                combinedColor += lights[i]->surfaceColor.toVec3f();
            }
            continue;
        }

        // create a shadow ray, pointing to the light source
        Vec3f light_direction = Vec3f::unitVector(lights[i]->center - hitRec.point);
        Vec3f origin = hitRec.point + hitRec.normal * 0.01f;
        Ray shadowRay = Ray(origin, light_direction);

        // determine if the shadow ray was blocked
        bool blocked = false;
        HitRecord reflectLightHitRec;
        HitRecord reflectObjHitRec;
        int closestReflectLight_i = findClosestIntersecting(shadowRay, lights, reflectLightHitRec);
        int closestReflectObject_i = findClosestIntersecting(shadowRay, objects, reflectObjHitRec);

        // if the shadow ray intersected an object
        if (closestReflectObject_i != -1)
        {
            blocked = true;
            // if the shadow ray also intersected a light
            if (closestReflectLight_i != -1)
            {
                // if the intersected light is closer than the intersected object, shadow ray not blocked
                float lightDistance = Vec3f::distance(shadowRay.origin, reflectLightHitRec.point);
                float objectDistance = Vec3f::distance(shadowRay.origin, reflectObjHitRec.point);
                if (lightDistance < objectDistance)
                {
                    blocked = false;
                }
            }
        }

        // if the shadow ray was blocked, don't compute the object's surface light
        if (blocked)
        {
            continue;
        }

        // calculate and contribute the lambertian color for the intersected object
        // ratio of light that the object deflects
        float objectAlbedo = 1.0f;

        Vec3f illumination = lights[i]->surfaceColor.toVec3f();
        float diffuseCoefficient = (objectAlbedo / 3.141592654f);
        float lightIntensity = lights[i]->emission;
        Vec3f objectColor = objects[closestIndex]->surfaceColor.toVec3f();
        float normalDotLight = std::max(0.0f, hitRec.normal.dotProduct(light_direction));

        Vec3f hitColor = illumination;
        hitColor *= diffuseCoefficient;
        hitColor *= lightIntensity;
        hitColor *= normalDotLight;

        hitColor.setX(hitColor.getX() * objectColor.getX());
        hitColor.setY(hitColor.getY() * objectColor.getY());
        hitColor.setZ(hitColor.getZ() * objectColor.getZ());

        combinedColor += hitColor;

    }

    return RGB_Color::toColor(combinedColor);

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

