#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <windows.h>
#include <stdio.h>
#include "gl.h"
#include "glu.h"
#include "glut.h"

#include "SceneLib.h"
#include "SceneRenderLib.h"
#include "SceneRenderOpenGlLib.h"
#include "Constants.h"
#include "GLUTMouseCallbacks.h"
#include "Timer.h"

void display(void);
void redisplay(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

/* this function establishes the projection matrix
   using either openGL calls or
   software calls which you will fill in */
void computeProjectionMatrix(void);

/* this function establishes the view matrix
   using either openGL calls or
   software calls which you will fill in */
void computeViewMatrix(void);

#endif  /* CALLBACKS_H */
