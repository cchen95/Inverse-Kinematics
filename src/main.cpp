

// Link with: opengl32.lib, glu32.lib, glut32.lib.

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#endif

#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <strstream>
#include <iomanip>
#include "arm.h"
#include "joint.h"
#include "Eigen/Dense"

using namespace Eigen;
using namespace std;

// Initial size of graphics window.
const int WIDTH  = 600;
const int HEIGHT = 400;

// Current size of window.
int width  = WIDTH;
int height = HEIGHT;

// Mouse positions, normalized to [0,1].
double xMouse = 0.5;
double yMouse = 0.5;

// Bounds of viewing frustum.
float nearPlane =  1.0;
float farPlane  = 1000.0;

// Viewing angle.
double view_angle = 60.0;

// Variables.
double alpha = 0;                                  // Set by idle function.
double beta = 0;                                   // Set by mouse X.
double mousedist = - (farPlane - nearPlane) / 2;    // Set by mouse Y.

Arm arm;

Vector2f targetPoint = Vector2f::Zero();

void display ()
{
    glEnable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0, 0, -200);

    glRotatef(90, 0, 0, 1);

    beta = 180.0 * xMouse;
    glRotatef(beta, 0, 1, 0);
    alpha = 180.0 * yMouse;
    glRotatef(beta, 1, 0, 0);

    arm.draw();

    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslatef(0, -targetPoint(1), targetPoint(0));
    glutSolidSphere(3.0f, 8, 8);
    glPopMatrix();
    
    glFlush();
    glutSwapBuffers();
}

void idle ()
{
    glutPostRedisplay();
}

void timer(int i)
{
  arm.update();
  if(arm.isTargetResolved()) {
	  targetPoint = arm.getPointWithinRange(); arm.moveToPoint(targetPoint); 
  }

    glutTimerFunc(10, timer, i);
    glutPostRedisplay();
}

//note: still bugged
void mouseMovement (int mx, int my)
{

    xMouse = double(mx) / double(width);
    yMouse = 1 - double(my) / double(height);

    glutPostRedisplay();
}


void reshapeMainWindow (int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(view_angle, GLfloat(width) / GLfloat(height), nearPlane, farPlane);
}

// quits on hitting esc
void graphicKeys (unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
    }
}

int main (int argc, char **argv)
{
    srand ( time(NULL) );

    //add the joints into arm
    for (int i = 1; i <= 5; i++)
    {
        Joint *arm = new Joint(1,1,1);
        arm->mAngle = 3.14f/4;
        arm->mLength = 5*i;
        ::arm.addJoint(arm);
    }

    // GLUT initialization.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("IK");

    // Register call backs.
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeMainWindow);
    glutKeyboardFunc(graphicKeys);
    glutMotionFunc(mouseMovement);
    glutIdleFunc(idle);
    glutTimerFunc(60, timer, 0);

    // OpenGL initialization
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat global_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightModelfv(GL_AMBIENT_AND_DIFFUSE, global_ambient);

    // Enter GLUT loop.
    glutMainLoop();

    return 0;
}
