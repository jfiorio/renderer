#ifndef MOUSE_H
#define MOUSE_H

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "gl.h"
#include "glu.h"
#include "glut.h"
#include "Constants.h"

/* GLUT callback functions */
void mouseMotion(int x, int y);
void mouseEntry(int state);

/* Functions to move the viewpoint around.
   These functions are called by the GLUT
   keyboard callback handler. */
void moveForward(void);
void moveLeft(void);
void moveRight(void);
void moveBack(void);
void moveUp(void);
void moveDown(void);


/* these rotations assume the right-hand rule, so
   if the vector is pointing towards the viewer,
   the positive rotation is counterclockwise */
void rotateAboutX(double *vector, double angle);
//void rotateAboutY(float *vector, float angle);
void rotateAboutZ(double *vector, double angle);

void crossProduct(double *res_vector, double *vector1, double *vector2);

bool floatEqual(double a, double b);

#endif  /* MOUSE_H */
