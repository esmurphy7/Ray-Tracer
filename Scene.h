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

#define ANTI_ALIASING_SAMPLES  50
#define REFLECTION_DEPTH        4

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
    Scene(unsigned int width, unsigned int height, float focalLength);
    void addObject(SceneObject* object);
    void traceRays();

private:
    Camera camera;
    ImagePlane imgPlane;
    std::vector<SceneObject*> lights;
    RGB_Color calculateColor(Ray r, int& reflectionDepth);
    int findClosestIntersecting(Ray r, std::vector<SceneObject*> objectSet, HitRecord& hitRecord);
    SceneObject* findClosestIntersectingLightOrObject(Ray ray, HitRecord& hitRecord);
};

Scene::Scene(unsigned int width, unsigned int height, float magnifier)
{
    // initialize camera
    camera = {
            Vec3f(0.0f, 0.0f, 0.0f),    // position
            Vec3f(0.0f, 0.0f, -1.0f),   // forward
            Vec3f(0.0f, 1.0f, 0.0f),    // up
            Vec3f(1.0f, 0.0f, 0.0f),    // right
    };

    // initialize image plane at position specified by magnifier factor
    Vec3f pos = camera.position + Vec3f(0.0f, 0.0f, -1.0f*magnifier);
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
    if(object->isLight())
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
                int reflectDepth = 0;
                RGB_Color colorSample = calculateColor(ray, reflectDepth);
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
RGB_Color Scene::calculateColor(Ray ray, int& reflectionDepth)
{
    HitRecord hitRec;
    SceneObject* closestObject = findClosestIntersectingLightOrObject(ray, hitRec);

    // if the ray didn't intersect an object or a light, return background color
    if (closestObject == nullptr)
    {
        return COLOR_BACKGROUND;
    }

    // if the ray intersected a light directly, return the light's color
    if(closestObject->isLight())
    {
        return closestObject->surfaceColor;
    }

    switch(closestObject->material)
    {
        // if the ray intersected a reflective surface, recursively trace the reflection ray
        case Material::REFLECTIVE:
        {
            // if the current depth of reflection has been reached, stop recursing
            if (reflectionDepth > REFLECTION_DEPTH)
            {
                return COLOR_BACKGROUND;
            }

            // R = I - 2 * dotProduct(I, N) * N;
            Vec3f reflectDir = ray.direction - hitRec.normal * 2 * Vec3f::dotProduct(ray.direction, hitRec.normal);
            Vec3f reflectOrig = hitRec.point + hitRec.normal * 0.01f;
            Ray reflectRay = Ray(reflectOrig, reflectDir);

            // return the reflection color, accounting for loss of brightness
            reflectionDepth++;
            return calculateColor(reflectRay, reflectionDepth) * 0.6f;
            break;
        }
        // if the ray intersected a diffuse surface, compute the combined color from all light sources
        case Material::DIFFUSE:
        {
            // iterate over all light sources
            Vec3f combinedColor = Vec3f(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < lights.size(); i++)
            {
                // create a shadow ray, pointing to the light source
                Vec3f light_direction = Vec3f::unitVector(lights[i]->center - hitRec.point);
                Vec3f origin = hitRec.point + hitRec.normal * 0.01f;
                Ray shadowRay = Ray(origin, light_direction);

                // determine if the shadow ray was blocked by a non-light object
                HitRecord shadHitRec;
                SceneObject *closestShadObject = findClosestIntersectingLightOrObject(shadowRay, shadHitRec);
                if (!closestShadObject->isLight())
                {
                    continue;
                }

                // calculate and contribute the lambertian color for the intersected object
                // ratio of light that the object deflects
                float objectAlbedo = 1.0f;

                Vec3f illumination = lights[i]->surfaceColor.toVec3f();
                float diffuseCoefficient = (objectAlbedo / 3.141592654f);
                float lightIntensity = lights[i]->emission;
                Vec3f objectColor = closestObject->surfaceColor.toVec3f();
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
            break;
        }
    }


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

/*
 * Find and return the closest object or light that intersects the ray.
 * Returns nullptr if neither a light nor an object intersects the ray.
 */
SceneObject* Scene::findClosestIntersectingLightOrObject(Ray ray, HitRecord& hitRecord)
{
    HitRecord objectRec;
    HitRecord lightRec;

    float objectDistance = std::numeric_limits<float>::max();
    float lightDistance = std::numeric_limits<float>::max();

    int closestObject_i = findClosestIntersecting(ray, objects, objectRec);
    int closestLight_i = findClosestIntersecting(ray, lights, lightRec);

    // find and return the object/light that is closest to the ray, update the hit record accordingly
    if(closestObject_i != -1 )
    {
        objectDistance = Vec3f::distance(ray.origin, objectRec.point);
    }
    if(closestLight_i != -1)
    {
        lightDistance = Vec3f::distance(ray.origin, lightRec.point);
    }
    if(objectDistance < lightDistance)
    {
        hitRecord = objectRec;
        return objects[closestObject_i];
    }
    else if(objectDistance > lightDistance)
    {
        hitRecord = lightRec;
        return lights[closestLight_i];
    }
    else
    {
        return nullptr;
    }
}
#endif //CSC305_A1_SCENE_H

