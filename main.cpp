#include <iostream>
#include <GL/freeglut.h>
#include <fstream>
#include "Scene.h"
#include "Sphere.h"
#include "Vec3.h"

using namespace std;

#define WINDOW_WIDTH    500
#define WINDOW_HEIGHT   500
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

void renderToPPM()
{
    // open ppm file
    ofstream ppmFile;
    ppmFile.open ("csc305-a1.ppm");

    std::vector<std::vector<RGB_Color>> pixelmap = scene->pixelmap;

    // write the ppm header to the file
   ppmFile << "P3\n" << pixelmap.size() << " " << pixelmap[0].size() << "\n255\n";

    // render each pixel in the scene's pixelmap
    for(int i=0; i<pixelmap.size(); i++)
    {
        for(int j=0; j<pixelmap[i].size(); j++)
        {
            float r = float(j) / float(pixelmap.size());
            float g = float(i) / pixelmap[i].size();
            float b = 0.2f;
            int ir = int(255.99*r);
            int ig = int(255.99*g);
            int ib = int(255.99*b);

            ppmFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    // close ppm image file
    ppmFile.close();
}

int main(int argc, char **argv)
{
    // build objects
    // center, surface color, emission, radius
    Sphere sphere_1 = Sphere(Vec3f(20.0f,20.0f,20.0f), RGB_Color(1,0,0), 0.0f, 10.0f);

    // insert each object into the scene
    scene = new Scene(WINDOW_WIDTH, WINDOW_HEIGHT, FOCAL_LENGTH);
    SceneObject* object = &sphere_1;
    scene->objects.push_back(object);

    // ray trace
    scene->traceRays();

    renderToPPM();

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