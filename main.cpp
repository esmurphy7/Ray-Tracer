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
#define FOCAL_LENGTH    1

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
    // build objects
    // center, surface color, emission, radius
    Sphere sphere_1 = Sphere(Vec3f(1.0f,-1.0f,-4.0f), RGB_Color(1.0f,0.0f,0.0f), 0.0f, 1.0f);
    Sphere sphere_2 = Sphere(Vec3f(2.0f,2.0f,-6.0f), RGB_Color(0.0f,0.0f,1.0f), 0.0f, 1.0f);
    // plane
    // center, normal, surface color, emission
    Plane plane_1 = Plane(Vec3f(0.0f,-100.0f,0.0f), Vec3f(0.0f,1.0f,-0.3f), RGB_Color(0.5f,1.0f,0.5f), 0.0f);
    // light emitting sphere
    Sphere light_sphere = Sphere(Vec3f(4.0f,2.0f,-4.0f), RGB_Color(1.0f,1.0f,1.0f), 1.0f, 0.5f);
    Sphere light_sphere2 = Sphere(Vec3f(-3.0f,-2.0f,-4.0f), RGB_Color(1.0f,1.0f,1.0f), 1.0f, 0.5f);

    // insert each object into the scene
    scene = new Scene(SCENE_WIDTH, SCENE_HEIGHT, FOCAL_LENGTH);
    scene->addObject(&sphere_1);
    scene->addObject(&sphere_2);
    scene->addObject(&plane_1);
    scene->addObject(&light_sphere);
    //scene->addObject(&light_sphere2);

    // ray trace
    scene->traceRays();

    renderToPPM(scene->pixelmap);
}