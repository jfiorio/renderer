#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Scene.h"

#define PI 3.1415926535897932384626433832795

Scene::Scene()
{  
   trianglePool = new Pool<Triangle>(8000);
   fragmentPool = new Pool<Fragment>(0x1000000); 
   
   textureCount = 0;
   bilerp_flag = false;

   projection = Matrix(1.0); /* Identity matrix */
         view = Matrix(1.0);
        model = Matrix(1.0);
   light = NULL;
   frontFacingIsCCW  = true;
   doBackFaceCulling = false;
   doLighting        = false;
   doSmoothLighting  = true;
   doOpenglFlatLighting = 0;
   useHalfVector     = false;
   doSmoothShading   = true;
   doTransparancy    = false;

   vp_width  = 100;  /* default Viewport width  */
   vp_height = 100;  /* default Viewport height */

}// default constructor

#include <new>
Scene::Scene(Scene *scene1) /* duplicate a Scene object */
{   
   /* copy information from scene1 to scene */
   for (int i = 0; i < scene1->textureCount; i++)
   {
      this->textures[i] = scene1->textures[i];
   }
   this->textureCount         = scene1->textureCount;
   this->bilerp_flag          = scene1->bilerp_flag;
   this->projection           = scene1->projection;
   this->view                 = scene1->view;
   this->model                = scene1->model;
   this->light                = scene1->light;
   this->frontFacingIsCCW     = scene1->frontFacingIsCCW;
   this->doBackFaceCulling    = scene1->doBackFaceCulling;
   this->doLighting           = scene1->doLighting;
   this->doSmoothLighting     = scene1->doSmoothLighting;
   this->doOpenglFlatLighting = scene1->doOpenglFlatLighting;
   this->useHalfVector        = scene1->useHalfVector;
   this->doSmoothShading      = scene1->doSmoothShading;
   this->doTransparancy       = scene1->doTransparancy;
   this->vp_width             = scene1->vp_width;
   this->vp_height            = scene1->vp_height;
   
   this->trianglePool         = scene1->trianglePool;
   this->fragmentPool         = scene1->fragmentPool;
   
   /* copy each Triangle object in the list from scene1 into the list for scene */
   for (ListNode<Triangle> *ptr = scene1->triangles.head; ptr; ptr = ptr->next)
   {
      ListNode<Triangle> *newT = trianglePool->allocateNode(); 
      newT = (ListNode<Triangle>*)new((void*)newT) Triangle( (ptr->v)+0, (ptr->v)+1, (ptr->v)+2 ); // placement new
      newT->tex = ptr->tex;
      newT->mtrl = ptr->mtrl;
      newT->next = 0;
      triangles.add(newT);
   }
   
}//duplicating constructor

Scene::~Scene()
{
  // !
}

void Scene::addTriangle(Triangle *t)
{
   ListNode<Triangle> *newT = trianglePool->allocateNode();
   newT = (ListNode<Triangle>*)new((void*)newT) Triangle( (t->v)+0, (t->v)+1, (t->v)+2 ); // placement new
   newT->tex = t->tex;
   newT->mtrl = t->mtrl;
   newT->next = 0;
   triangles.add(newT);   
}

void Scene::addTexture(Texture * tex)
{
   textures[textureCount++] = tex;
   return;
}
 
void Scene::proj2Identity()
{
   projection = Matrix(1.0);
   return;
}


void Scene::projMult(Matrix m)
{
   projection = projection.times(m);
   return;
}


void Scene::projPerspective(float fovy, float aspect, float zNear, float zFar)
{
   /*  http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml  */
   float f = 1.0/tan((PI/180.0)*fovy/2.0);
   projection = projection.times(Matrix(Vector(f/aspect, 0.0, 0.0, 0.0),
                                        Vector(0.0, f, 0.0, 0.0),
                                        Vector(0.0, 0.0, (zFar+zNear)/(zNear-zFar), -1.0),
                                        Vector(0.0, 0.0, (2.0*zFar*zNear)/(zNear-zFar), 0.0)));
   return;
}


void Scene::projFrustum(float left, float right, float bottom, float top, float near, float far)
{
   /*  http://www.opengl.org/sdk/docs/man/xhtml/glFrustum.xml  */
   float A = (right+left)/(right-left);
   float B = (top+bottom)/(top-bottom);
   float C =   (far+near)/(far-near);
   float D = (2*far*near)/(far-near);
   projection = projection.times(Matrix(Vector((2.0*near)/(right-left), 0.0, 0.0, 0.0),
                                        Vector(0.0, (2.0*near)/(top-bottom), 0.0, 0.0),
                                        Vector(  A,   B,  C, -1.0),
                                        Vector(0.0, 0.0,  D,  0.0)));
   return;
}


void Scene::projOrtho(float left, float right, float bottom, float top, float near, float far)
{
   /*  http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml  */
   float tx = -(right+left)/(right-left);
   float ty = -(top+bottom)/(top-bottom);
   float tz =   -(far+near)/(far-near);
   projection = projection.times(Matrix(Vector((2.0)/(right-left), 0.0, 0.0, 0.0),
                                        Vector(0.0, (2.0)/(top-bottom), 0.0, 0.0),
                                        Vector(0.0, 0.0, (-2.0)/(far-near), 0.0),
                                        Vector(tx, ty, tz, 1.0)));
   return;
}


void Scene::view2Identity()
{
   view = Matrix(1.0);
   return;
}


void Scene::viewLookAt(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
   /*  http://www.opengl.org/sdk/docs/man/xhtml/gluLookAt.xml  */
   Vector F  = Vector(centerx - eyex, centery - eyey, centerz - eyez);
   Vector UP = Vector(upx, upy, upz);
   Vector f  = F.normalize();
   Vector up = UP.normalize();
   Vector s  = f.crossProduct(up);
   Vector u  = s.crossProduct(f);
   view = view.times(Matrix(Vector(s.x, u.x, -f.x, 0.0),
                            Vector(s.y, u.y, -f.y, 0.0),
                            Vector(s.z, u.z, -f.z, 0.0),
                            Vector(0.0, 0.0,  0.0, 1.0)));

   view = view.times(Matrix(Vector(-eyex, -eyey, -eyez)));
   return;
}


void Scene::viewRotate(float theta, float x, float y, float z)
{
   view = view.times(Matrix(theta, x, y, z));
   return;
}


void Scene::viewTranslate(float x, float y, float z)
{
   view = view.times(Matrix(Vector(x, y, z)));
   return;
}


void Scene::model2Identity()
{
   model = Matrix(1.0);
   return;
}


void Scene::modelMult(Matrix m)
{
   model = model.times(m);
   return;
}


void Scene::modelRotate(float theta, float x, float y, float z)
{
   model = model.times(Matrix(theta, x, y, z));
   return;
}


void Scene::modelScale(float x, float y, float z)
{
   model = model.times(Matrix(x, y, z));
   return;
}


void Scene::modelTranslate(float x, float y, float z)
{
   model = model.times(Matrix(Vector(x, y, z)));
   return;
}


void Scene::setViewport(int width, int height)
{
   vp_width  = width;
   vp_height = height;
}


/* for debugging */
void Scene::print(int fields)
{
   //fprintf(stderr, "Printing out all Scene's Triangles:\n");
   fflush(stderr);
   for (ListNode<Triangle> *ptr = triangles.head; ptr; ptr = ptr->next)
   {
      ptr->print(fields);
   }
   //fprintf(stderr, "Done printing out Scene\n");
   fflush(stderr);
}
