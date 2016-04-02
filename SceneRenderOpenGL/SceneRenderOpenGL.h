#ifndef RENDEROPENGL_H
#define RENDEROPENGL_H

#include <windows.h>
#include "gl.h"
#include "glu.h"

#include "SceneLib.h"

void renderSceneOpenGL(Scene *s);
void renderTriangleOpenGL(Triangle *t, bool doLighting);
void initializeTexturesOpenGL(Scene *s);
void switchTextureFilteringOpenGL(Scene *s);
void checkErrorsOpenGL(const char *prefix);
void printOpenGLMatrix(GLenum pname);

#endif /* RENDEROPENGL_H */
