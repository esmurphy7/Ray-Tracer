#include <iostream>
#include <fstream>
#include "Scene.h"
#include "Sphere.h"
#include "Vec3.h"
#include "PPM_File.h"
#include "Plane.h"

using namespace std;

#define SCENE_WIDTH    200
#define SCENE_HEIGHT   100
#define MAGNIFIER      1

Scene* scene;

void renderToPPM(std::vector<std::vector<RGB_Color>> pixelmap)
{
    // open ppm file
    PPM_File ppmFile;
    // filename, width, height
    ppmFile.open("csc305-a1.ppm", pixelmap[0].size(), pixelmap.size());

    // render each pixel in the scene's pixelmap
    for(int i=pixelmap.size()-1; i>=0; i--)
    {
        for(int j=pixelmap[i].size()-1; j>=0; j--)
        {
            RGB_Color color = pixelmap[i][j];
            ppmFile.writeColor(color.R, color.G, color.B);
        }
    }

    // close ppm image file
    ppmFile.close();
}

int main(int argc, char **argv)
{
    // spheres
    // center, surface color, emission, radius
    Sphere sphere_1 = Sphere(Vec3f(1.0f,-1.0f,-4.0f), RGB_Color(1.0f,0.0f,0.0f), 0.0f, 1.0f);
    Sphere sphere_2 = Sphere(Vec3f(2.0f,2.0f,-10.0f), RGB_Color(0.0f,0.0f,1.0f), 0.0f, 1.5f);
    Sphere sphere_3 = Sphere(Vec3f(-3.0f,3.0f,-6.0f), RGB_Color(0.3f,0.5f,0.4f), 0.0f, 1.5f);

    // planes
    // center, normal, surface color, emission
    Plane floor = Plane(Vec3f(0.0f,-20.0f,0.0f), Vec3f(0.0f,1.0f,-0.3f), RGB_Color(0.0f,0.5f,0.1f), 0.0f);
    Plane far_wall = Plane(Vec3f(0.0f,0.0f,-100.0f), Vec3f(0.0f,0.0f, 1.0f), RGB_Color(0.3f,0.0f,0.6f), 0.0f);
    Plane left_wall = Plane(Vec3f(100.0f,0.0f,0.0f), Vec3f(-1.0f,0.0f, 0.0f), RGB_Color(0.6f,0.8f,0.0f), 0.0f);
    Plane right_wall = Plane(Vec3f(-100.0f,0.0f,0.0f), Vec3f(1.0f,0.0f, 0.0f), RGB_Color(0.0f,0.6f,0.8f), 0.0f);
    Plane roof = Plane(Vec3f(0.0f,40.0f,0.0f), Vec3f(0.0f,-1.0f, 0.0f), RGB_Color(0.0f,0.6f,0.8f), 0.0f);

    // lights
    // center, surface color, emission, radius
    Sphere light_sphere = Sphere(Vec3f(-1.0f, 1.0f, -3.0f), RGB_Color(1.0f, 1.0f, 1.0f), 1.0f, 0.1f);
    Sphere light_sphere2 = Sphere(Vec3f(2.0f,1.0f,-3.0f), RGB_Color(1.0f,1.0f,1.0f), 2.0f, 0.1f);

    // insert each object into the scene
    scene = new Scene(SCENE_WIDTH, SCENE_HEIGHT, MAGNIFIER);
    scene->addObject(&sphere_1);
    scene->addObject(&sphere_2);
    scene->addObject(&sphere_3);

    scene->addObject(&floor);
    scene->addObject(&far_wall);
    scene->addObject(&left_wall);
    scene->addObject(&right_wall);
    scene->addObject(&roof);

    scene->addObject(&light_sphere);
    scene->addObject(&light_sphere2);

    // ray trace
    scene->traceRays();

    renderToPPM(scene->pixelmap);
}