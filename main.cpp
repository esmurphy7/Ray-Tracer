#include <iostream>
#include <GL/freeglut.h>
#include "Scene.h"
#include "Sphere.h"

using namespace std;

void buildScene()
{
    // build each object
    //Sphere* sphere_1 = new Sphere(Vect3(1,1,1), RGBColor(0,0,0), )

    // insert each object into the scene
    Scene* scene = new Scene(10, 10);
}

void render()
{

}

int main(int argc, char **argv)
{
    // init and build window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("CSC305-A1");

    //buildScene();

    //render();

    // save image
}