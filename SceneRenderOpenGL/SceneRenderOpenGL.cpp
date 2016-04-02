#include <stdio.h>
#include "SceneRenderOpenGL.h"

void renderSceneOpenGL(Scene *s)
{
   GLdouble m1[]={s->view.v1.x, s->view.v1.y, s->view.v1.z, s->view.v1.w,
                  s->view.v2.x, s->view.v2.y, s->view.v2.z, s->view.v2.w,
                  s->view.v3.x, s->view.v3.y, s->view.v3.z, s->view.v3.w,
                  s->view.v4.x, s->view.v4.y, s->view.v4.z, s->view.v4.w};

   GLdouble m2[]={s->model.v1.x, s->model.v1.y, s->model.v1.z, s->model.v1.w,
                  s->model.v2.x, s->model.v2.y, s->model.v2.z, s->model.v2.w,
                  s->model.v3.x, s->model.v3.y, s->model.v3.z, s->model.v3.w,
                  s->model.v4.x, s->model.v4.y, s->model.v4.z, s->model.v4.w};

   GLdouble m3[]={s->projection.v1.x, s->projection.v1.y, s->projection.v1.z, s->projection.v1.w,
                  s->projection.v2.x, s->projection.v2.y, s->projection.v2.z, s->projection.v2.w,
                  s->projection.v3.x, s->projection.v3.y, s->projection.v3.z, s->projection.v3.w,
                  s->projection.v4.x, s->projection.v4.y, s->projection.v4.z, s->projection.v4.w};

   /* use the Scene object to set OpenGL's viewport */
   glViewport(0, 0, s->vp_width, s->vp_height);

   /* use the Scene object to set OpenGL's lighting, shading, culling and orientation modes */
   if (s->doLighting && s->light)
   {
      glEnable(GL_LIGHTING);
      glEnable(GL_NORMALIZE);
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0, GL_AMBIENT,  s->light->ambient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE,  s->light->diffuse);
      glLightfv(GL_LIGHT0, GL_SPECULAR, s->light->specular);
   }
   else
   {
      glDisable(GL_LIGHTING);
   }

   if (s->doSmoothLighting)
   {
      glShadeModel(GL_SMOOTH);
   }
   else
   {
      glShadeModel(GL_FLAT);
   }

   if (s->doBackFaceCulling)
   {
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
   }
   else
   {
      glDisable(GL_CULL_FACE);
   }

   if (s->frontFacingIsCCW)
   {
      glFrontFace(GL_CCW);
   }
   else
   {
      glFrontFace(GL_CW);
   }

   //http://stackoverflow.com/questions/4124041/is-opengl-coordinate-system-left-handed-or-right-handed
   //glDepthRange(1, 0);


   /* use the Scene object to set OpenGL's modelview matrix */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glMultMatrixd( (const GLdouble*)&m1 );  // change world to view coordinates

   /* if we are using a light, convert it from world to view coordinates */
   if (s->doLighting && s->light)
   {
      glLightfv(GL_LIGHT0, GL_POSITION, s->light->pos);
   }

   glMultMatrixd( (const GLdouble*)&m2 );  // change model to world coordinates


   /* use the Scene object to set OpenGL's projection matrix */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glMultMatrixd( (const GLdouble*)&m3 );

   glClearColor(100/(float)255, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);

#if DEBUG1
   fprintf(stderr, "================= OpenGL begin =============\n");
   fflush(stderr);
#endif /* DEBUG1 */

   /* render each Triangle from the list of Triangle objects */
   for (TriangleListNode *ptr = (s->head_node).next;  ptr;  ptr = ptr->next)
   {
      renderTriangleOpenGL(ptr->t, s->doLighting);
   }

#if DEBUG1
   fprintf(stderr, "================= OpenGL end ===============\n");
   fflush(stderr);
#endif /* DEBUG1 */

   glFinish(); /* wait while everything gets rendered */

   glDisable(GL_DEPTH_TEST);

   checkErrorsOpenGL("Errors in rendering scene!");

   return;
}//renderSceneOpenGL()


void renderTriangleOpenGL(Triangle *t, bool doLighting)
{
   if (t->tex)
   {
      glBindTexture(GL_TEXTURE_2D, t->tex->tex);
      glEnable(GL_TEXTURE_2D);
   }

   if (t->mtrl)
   {
      glMaterialfv(GL_FRONT, GL_AMBIENT,   t->mtrl->ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE,   t->mtrl->diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR,  t->mtrl->specular);
      glMaterialf (GL_FRONT, GL_SHININESS, t->mtrl->s);
   }

   /* now draw the triangle */
   glBegin(GL_TRIANGLES);
      if (t->tex)      glTexCoord2f(t->v[0].s,   t->v[0].t);
      if (!doLighting) glColor3f(   t->v[0].r,   t->v[0].g,   t->v[0].b );
      glNormal3f( t->v[0].n.x, t->v[0].n.y, t->v[0].n.z );
      glVertex3f( t->v[0].x,   t->v[0].y,   t->v[0].z );

      if (t->tex)      glTexCoord2f(t->v[1].s,   t->v[1].t);
      if (!doLighting) glColor3f(   t->v[1].r,   t->v[1].g,   t->v[1].b );
      glNormal3f( t->v[1].n.x, t->v[1].n.y, t->v[1].n.z );
      glVertex3f( t->v[1].x,   t->v[1].y,   t->v[1].z );

      if (t->tex)      glTexCoord2f(t->v[2].s,   t->v[2].t);
      if (!doLighting) glColor3f(   t->v[2].r,   t->v[2].g,   t->v[2].b );
      glNormal3f( t->v[2].n.x, t->v[2].n.y, t->v[2].n.z );
      glVertex3f( t->v[2].x,   t->v[2].y,   t->v[2].z );
   glEnd();

#if DEBUG1
      fprintf(stderr, "[% .6f % .6f % .6f % .6f]\n", t->v[0].x, t->v[0].y, t->v[0].z, t->v[0].w );
      fprintf(stderr, "{% .6f % .6f % .6f % .6f}\n", t->v[1].x, t->v[1].y, t->v[1].z, t->v[1].w );
      fprintf(stderr, "<% .6f % .6f % .6f % .6f>\n", t->v[2].x, t->v[2].y, t->v[2].z, t->v[2].w );
      if (t->tex){
      fprintf(stderr, "[% .6f % .6f]\n", t->v[0].s, t->v[0].t );
      fprintf(stderr, "{% .6f % .6f}\n", t->v[1].s, t->v[1].t );
      fprintf(stderr, "<% .6f % .6f>\n", t->v[2].s, t->v[2].t ); }
      fflush(stderr);
#endif /* DEBUG1 */

   if (t->tex)
   {
      glDisable(GL_TEXTURE_2D);
   }

   checkErrorsOpenGL("Errors in rendering triangle!");

   return;
}//renderTriangleOpenGL()


void initializeTexturesOpenGL(Scene *s)
{
   for (int i = 0; i < s->textureCount; i++)
   {
      glGenTextures(1, &(s->textures[i]->tex));
      glBindTexture(GL_TEXTURE_2D, s->textures[i]->tex);
      glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

      if (s->bilerp_flag)
      {
         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
      else
      {
         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }

      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, s->textures[i]->width,
                                              s->textures[i]->height,
                                              0, GL_RGB, GL_UNSIGNED_BYTE,
                                              s->textures[i]->data );
   }

   checkErrorsOpenGL("Errors in initializing textures!");

   return;
}//initializeTexturesOpenGL()


void switchTextureFilteringOpenGL(Scene *s)
{
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

   checkErrorsOpenGL("Errors in switching texture filter!");

   return;
}//switchTextureFilteringOpenGL()


void printOpenGLMatrix(GLenum pname) // GL_PROJECTION_MATRIX or GL_MODELVIEW_MATRIX
{
   GLdouble m[16];
   glGetDoublev(pname, m);
   fprintf(stderr,"{{% .6f % .6f % .6f % .6f}\n",  m[0], m[4], m[ 8], m[12]);
   fprintf(stderr," {% .6f % .6f % .6f % .6f}\n",  m[1], m[5], m[ 9], m[13]);
   fprintf(stderr," {% .6f % .6f % .6f % .6f}\n",  m[2], m[6], m[10], m[14]);
   fprintf(stderr," {% .6f % .6f % .6f % .6f}}\n", m[3], m[7], m[11], m[15]);
   fflush(stderr);
}


void checkErrorsOpenGL(const char *prefix)
{
   GLenum error;
   if ((error = glGetError()) != GL_NO_ERROR)
   {
      fprintf(stderr,"%s: gl error: %s\n",prefix, gluErrorString(error));
      exit(-1);
   }
   return;
}//checkErrorsOpenGL()
