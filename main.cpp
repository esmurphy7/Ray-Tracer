#include <iostream>
#include <GL/freeglut.h>
#include <fstream>
#include "Scene.h"
#include "Sphere.h"
#include "Vec3.h"
#include "PPM_File.h"

using namespace std;

#define WINDOW_WIDTH    200
#define WINDOW_HEIGHT   100
#define FOCAL_LENGTH    10

Scene* scene;

void renderToGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render each pixel in the scene's pixelmap to the glut window
    std::vector<std::vector<RGB_Color>> pixelmap = scene->pixelmap;
    for(int i=0; i<pixelmap.size(); i++)
    {
        for(int j=0; j<pixelmap[i].size(); j++)
        {
            glColor3f(pixelmap[i][j].R, pixelmap[i][j].G, pixelmap[i][j].B);
        }
    }

    glutSwapBuffers();
}

void renderToPPM(std::vector<std::vector<RGB_Color>> pixelmap)
{
    // open ppm file
    PPM_File ppmFile;
    // filename, width, height
    ppmFile.open("csc305-a1.ppm", pixelmap[0].size(), pixelmap.size());

    // render each pixel in the scene's pixelmap
    for(int i=0; i<pixelmap.size(); i++)
    {
        for(int j=0; j<pixelmap[i].size(); j++)
        {
            float r = float(j) / float(pixelmap.size());
            float g = float(i) / pixelmap[i].size();
            float b = 0.2f;
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
    Sphere sphere_1 = Sphere(Vec3f(0.0f,0.0f,-1.0f), RGB_Color(1.0f,0.0f,0.0f), 0.0f, 0.3f);
    Sphere sphere_2 = Sphere(Vec3f(0.5f,0.5f,-1.0f), RGB_Color(0.0f,0.0f,0.0f), 0.0f, 0.3f);

    // insert each object into the scene
    scene = new Scene(WINDOW_WIDTH, WINDOW_HEIGHT, FOCAL_LENGTH);
    SceneObject* object = &sphere_1;
    scene->objects.push_back(object);
    object = &sphere_2;
    scene->objects.push_back(object);

    // ray trace
    scene->traceRays();

    renderToPPM(scene->pixelmap);

    // init and build window
    /*
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutCreateWindow("CSC305-A1");
    glutDisplayFunc(renderToGL);
    glutMainLoop();
     */
}