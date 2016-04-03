#ifndef SCENE_H
#define SCENE_H

#include "Triangle.h"
#include "Texture.h"
#include "Matrix.h"
#include "Light.h"
#include "List.h"

class Scene
{
   private:
   public:
      List<Triangle> triangles;
      Pool<Triangle> *trianglePool; 
      Pool<Fragment> *fragmentPool;
      
      Texture * textures[256];     /* an array of pointers to Texture objects */
      int textureCount;
      bool bilerp_flag;  /* Determines which rendering mode to use for textures. */
      Matrix projection;
      Matrix view;
      Matrix model;
      Light * light; /* data structure that describes a light */
      bool doBackFaceCulling;
      bool frontFacingIsCCW;
      bool doLighting;
      bool doSmoothLighting;
      int  doOpenglFlatLighting;  /* flat lighting in the OpenGL style */
      bool useHalfVector;
      bool doSmoothShading;
      bool doTransparancy;

      int vp_width;  /* Viewport width */
      int vp_height; /* Viewport height */

      Scene();
      Scene(Scene *scene);  // duplicate a Scene object

      ~Scene();  // ~Scene() calls ~Triangle() which calls ~Fragment().
                 // The Light, Texture, and Material objects must be deleted manually!!
      
      void addTriangle(Triangle * t);
      void addTexture(Texture * tex);

      void proj2Identity();
      void projMult(Matrix m);
      void projPerspective(float fovy, float aspect, float zNear, float zFar);
      void projFrustum(float left, float right, float bottom, float top, float near, float far);
      void projOrtho(float left, float right, float bottom, float top, float near, float far);

      void view2Identity();
      void viewLookAt(float eyex,    float eyey,    float eyez,
                      float centerx, float centery, float centerz,
                      float upx,     float upy,     float upz);
      void viewRotate(float theta, float x, float y, float z);
      void viewTranslate(float x, float y, float z);

      void model2Identity();
      void modelMult(Matrix m);
      void modelRotate(float theta, float x, float y, float z);
      void modelScale(float x, float y, float z);
      void modelTranslate(float x, float y, float z);

      void setViewport(int width, int height);

      /* for debugging */
      void print(int fields);
};

#endif /* SCENE_H */
