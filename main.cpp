#include <iostream>
#include <GL/freeglut.h>
#include "Scene.h"
#include "Sphere.h"

using namespace std;

#define WINDOW_WIDTH    500
#define WINDOW_HEIGHT   500
#define FOCAL_LENGTH    10

Scene* scene;

void render()
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

int main(int argc, char **argv)
{
    // build objects
    // center, surface color, emission, radius
    Sphere sphere_1 = Sphere(Vect3(20,20,20), RGB_Color(1,0,0), 0, 10);

    // insert each object into the scene
    scene = new Scene(WINDOW_WIDTH, WINDOW_HEIGHT, FOCAL_LENGTH);
    SceneObject object = sphere_1;
    scene->objects.push_back(object);

    // ray trace
    scene->generateRays();

    // init and build window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutCreateWindow("CSC305-A1");
    glutDisplayFunc(render);
    glutMainLoop();
}