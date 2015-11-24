#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <omp.h>
using namespace std;

#include <GL/glut.h>
#include <GL/glu.h>
#include "molecdyn.h"


#define SQRT_3 1.7320508075688773
#define SQRT_2_3 0.816496580927726

/*
 * compile: g++  -o animGL.exe MolecDynOpenGLAnim.cpp -fopenmp -I../include -lm -lGL -lGLU -lglut -lXi -lXmu -L/home/konrad/CProjects/kms-project1/lib -Wl,-R/home/konrad/CProjects/kms-project1/lib -lmolecdyn -O3
 
 */

//extern Global_t global;

int step;                 // keeps track of integration step number
int displayInterval = 10;  // display molecules every so many steps

void makeMolecules();     // this function re-draws the molecules

void takeStep() {
    evolve_system(displayInterval);
    makeMolecules();
    glutPostRedisplay();
    ++step;
    /*
    if (step % 200 == 0)
        rescaleVelocities();
    if (step % updateInterval == 0) {
        updatePairList();
        updatePairSeparations();
    }
    if (step % displayInterval == 0) {
        makeMolecules();
        glutPostRedisplay();
    }*/
}

const double pi = 4 * atan(1.0);
double radius = 0.5;                      // radius of molecule
double minExtent[3], maxExtent[3];        // extent of system volume
int xWindowSize = 640, yWindowSize = 640; // window size in screen pixels
GLdouble aspectRatio;                     // window aspect ratio
GLdouble fovy, nearClip, farClip;         // variables for 3D projection
GLdouble eye[3], center[3], up[3];        // more projection variables
GLuint sphereID, configID;                // display list ID numbers 
int phi, theta;                           // to rotate system using arrow keys
int angle = 5;                            // rotation angle in degrees

void makeSphere(GLuint listID, double radius) {
    int nTheta = 9;                       // number of polar angle slices
    int nPhi = 18;                        // number of azimuthal angle slices
    glNewList(listID, GL_COMPILE);
        glutSolidSphere(global.R, nPhi, nTheta);
    glEndList();
}

void makeMolecules() {
    glNewList(configID, GL_COMPILE);
    glColor3f(0.0, 0.0, 1.0);             // color the molecules blue
    glPushMatrix();
    glRotated(phi, 0, 1, 0);              // rotate about y axis
    glPushMatrix();
    glRotated(theta, 1, 0, 0);            // rotate about x axis
    for (uint16_t ii = 0; ii < global.N; ii++) {
        glPushMatrix();
        glTranslated(global.x_arr[ii], global.y_arr[ii], global.z_arr[ii]);
        glCallList(sphereID);
        glPopMatrix();
    }
    glColor3ub(255, 255, 255);            // white
    //glutWireCube(L);                      // cubical system volume
    glutWireSphere(global.L,20,20);
    glPopMatrix();
    glPopMatrix();
    glEndList();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], 
              center[0], center[1], center[2],
              up[0], up[1], up[2]);
    glCallList(configID);                 // draw molecules
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    aspectRatio = w / double(h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspectRatio, nearClip, farClip);
    glMatrixMode(GL_MODELVIEW);
}

void initView(double *minExtent, double *maxExtent) {

    // use a single light source to illuminate the scene
    GLfloat lightDiffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat lightPosition[] = {0.5, 0.5, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);              // use single light number 0
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    // compute the distance scale of the system
    double difExtent[3];
    for (int i = 0; i < 3; i++)
        difExtent[i] = maxExtent[i] - minExtent[i];
    double dist = 0;
    for (int i = 0; i < 3; i++)
        dist += difExtent[i] * difExtent[i];
    dist = sqrt(dist);

    // locate the center of the system, camera position, and orientation
    for (int i = 0; i < 3; i++)
        center[i] = minExtent[i] + difExtent[i] / 2;
    eye[0] = center[0];  
    eye[1] = center[1];  
    eye[2] = center[2] + dist;        // along z axis is towards viewer
    up[0] = 0;
    up[1] = 1;                        // y axis is up
    up[2] = 0;

    // set up clipping planes, field of view angle in degrees in y direction
    nearClip = (dist - difExtent[2] / 2) / 2;
    farClip = 2 * (dist + difExtent[2] / 2);
    fovy = difExtent[1] / (dist - difExtent[2] / 2) / 2;
    fovy = 2 * atan(fovy) / pi * 180;
    fovy *= 1.2;
}

void special(int key, int x, int y) {
    switch(key) {
      case GLUT_KEY_LEFT:   phi = (phi - angle) % 360;      break;
      case GLUT_KEY_RIGHT:  phi = (phi + angle) % 360;      break;
      case GLUT_KEY_UP:     theta = (theta - angle) % 360;  break;
      case GLUT_KEY_DOWN:   theta = (theta + angle) % 360;  break;
      default: break;
    }
}


double base_vec1[] = {       1.,          0.,          0.};
double base_vec2[] = {      0.5,   SQRT_3/2.,          0.};
double base_vec3[] = {      0.5,   SQRT_3/6.,    SQRT_2_3};

int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    
    const double a = 0.37317;
    const double R = 0.38;
    const double m = 39.948;
    const double T0 = 1000;
    const double epsilon = 1.;
    const double f = 1e05;
    const double L = 2.3;
    const double dt = 1e-04;
    const uint32_t nx = 5;
    const uint32_t ny = 5;
    const uint32_t nz = 5;
    init_csimulation(a, R, m, T0, epsilon, f, L, dt, nx, ny, nz, base_vec1, base_vec2, base_vec3);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(xWindowSize, yWindowSize);
    glutCreateWindow("Molecular Dynamics of Lennard-Jones Gas");
    
    for (int i = 0; i < 3; i++) {
        minExtent[i] = -global.L;
        maxExtent[i] = global.L;
    }
    initView(minExtent, maxExtent);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(takeStep);
    glutSpecialFunc(special);

    sphereID = glGenLists(1);
    makeSphere(sphereID, radius);
    configID = glGenLists(1);
    makeMolecules();

    glutMainLoop();
}