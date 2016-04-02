#include "SceneLib.h"

#include <windows.h>
#include <stdio.h>
#include "../gl/gl.h"
#include "../gl/glu.h"


void renderTriangleGL(Triangle *t);
void switchTextureFilteringGL(Scene *s);
void checkErrorsGL(const char *prefix);


void P8_Switch2OpenGL(Scene *scene)
{
   glDisable(GL_CULL_FACE); /* make sure our renderer is doing the culling */
   glDisable(GL_LIGHTING);  /* make sure our renderer is doing the lighting */
   glShadeModel(GL_SMOOTH); /* we need OpenGL to do the smooth shading for us */
   glEnable(GL_DEPTH_TEST);
   glClearColor(0, 100/(float)255, 100/(float)255, 0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   if ( !(scene->doLighting) && !(scene->doSmoothLighting) )
   {
      glShadeModel(GL_FLAT);
   }

   /* use the Scene object to set OpenGL's viewport */
   glViewport(0, 0, scene->vp_width, scene->vp_height);

   /* set OpenGL's modelview matrix */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   /* set OpenGL's projection matrix */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

   /* render each Triangle from the list of Triangle objects */
   for (TriangleListNode *ptr = (scene->head_node).next;  ptr;  ptr = ptr->next)
   {
      renderTriangleGL(ptr->t);
   }

   glFinish(); /* wait while everything gets rendered */

   checkErrorsGL("OpenGL errors in rendering scene!");

   return;
}


void renderTriangleGL(Triangle *t)
{
   if (t->tex)
   {
      glBindTexture(GL_TEXTURE_2D, t->tex->tex);
      glEnable(GL_TEXTURE_2D);
   }

   /* now draw the triangle */
   glBegin(GL_TRIANGLES);
      if (t->tex) glTexCoord2f(t->v[0].s, t->v[0].t);
      glColor3f ( t->v[0].r, t->v[0].g, t->v[0].b );
      glVertex3f( t->v[0].x, t->v[0].y, t->v[0].z );

      if (t->tex) glTexCoord2f(t->v[1].s, t->v[1].t);
      glColor3f ( t->v[1].r, t->v[1].g, t->v[1].b );
      glVertex3f( t->v[1].x, t->v[1].y, t->v[1].z );

      if (t->tex) glTexCoord2f(t->v[2].s, t->v[2].t);
      glColor3f ( t->v[2].r, t->v[2].g, t->v[2].b );
      glVertex3f( t->v[2].x, t->v[2].y, t->v[2].z );
   glEnd();

   if (t->tex)
   {
      glDisable(GL_TEXTURE_2D);
   }

   checkErrorsGL("Errors in rendering triangle!");

   return;
}//renderTriangleGL()


void switchTextureFilteringGL(Scene *s)
{
   /* do this with OpenGL for now */
   for (int i = 0; i < s->textureCount; i++)
   {
      glBindTexture(GL_TEXTURE_2D, s->textures[i]->tex);
      if (s->bilerp_flag)
      {
         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
      else
      {
         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }
   }

   return;
}//switchTextureFilteringGL()


void checkErrorsGL(const char *prefix)
{
   GLenum error;
   if ((error = glGetError()) != GL_NO_ERROR)
   {
      fprintf(stderr,"%s: gl error: %s\n",prefix, gluErrorString(error));
      exit(-1);
   }
   return;
}//checkErrorsGL()
