#include <iostream>
#include <GL/freeglut.h>
#include <fstream>
#include "Scene.h"
#include "Sphere.h"
#include "Vec3.h"
#include "PPM_File.h"

using namespace std;

#define SCENE_WIDTH    200
#define SCENE_HEIGHT   100
#define FOCAL_LENGTH    1

Scene* scene;

void renderToGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render each pixel in the scene's pixelmap to the glut window
    std::vector<std::vector<RGB_Color>> pixelmap = scene->pixelmap;
    for(int i=pixelmap.size()-1; i<=0; i--)
    {
        for(int j=pixelmap[i].size()-1; j>=0; j--)
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
    Sphere sphere_1 = Sphere(Vec3f(0.0f,-2.0f,-2.0f), RGB_Color(1.0f,0.0f,0.0f), 0.0f, 1.0f);
    Sphere sphere_2 = Sphere(Vec3f(-2.0f,2.0f,-6.0f), RGB_Color(0.0f,0.0f,1.0f), 0.0f, 1.0f);
    // light emitting sphere
    Sphere light_sphere = Sphere(Vec3f(4.0f,2.0f,-4.0f), RGB_Color(1.0f,1.0f,1.0f), 1.0f, 0.5f);
    Sphere light_sphere2 = Sphere(Vec3f(-3.0f,-2.0f,-4.0f), RGB_Color(1.0f,1.0f,1.0f), 1.0f, 0.5f);

    // insert each object into the scene
    scene = new Scene(SCENE_WIDTH, SCENE_HEIGHT, FOCAL_LENGTH);
    scene->addObject(&sphere_1);
    scene->addObject(&sphere_2);
    scene->addObject(&light_sphere);
    scene->addObject(&light_sphere2);

    // ray trace
    scene->traceRays();

    renderToPPM(scene->pixelmap);

    // init and build window
    /*
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(SCENE_WIDTH,SCENE_HEIGHT);
    glutCreateWindow("CSC305-A1");
    glutDisplayFunc(renderToGL);
    glutMainLoop();
     */
}