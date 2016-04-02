#ifndef SCENE_H
#define SCENE_H

#include "TriangleListNode.h"
#include "Triangle.h"
#include "Texture.h"
#include "Matrix.h"
#include "Light.h"

class Scene
{
   private:
   public:
      TriangleListNode head_node;  /* a linked list of triangles */
      TriangleListNode *tail_ptr;
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

      void addTriangle(Triangle * tri);

      void addTexture(Texture * tex);

      void proj2Identity();
      void projMult(Matrix m);
      void projPerspective(double fovy, double aspect, double zNear, double zFar);
      void projFrustum(double left, double right, double bottom, double top, double near, double far);
      void projOrtho(double left, double right, double bottom, double top, double near, double far);

      void view2Identity();
      void viewLookAt(double eyex,    double eyey,    double eyez,
                      double centerx, double centery, double centerz,
                      double upx,     double upy,     double upz);
      void viewRotate(double theta, double x, double y, double z);
      void viewTranslate(double x, double y, double z);

      void model2Identity();
      void modelMult(Matrix m);
      void modelRotate(double theta, double x, double y, double z);
      void modelScale(double x, double y, double z);
      void modelTranslate(double x, double y, double z);

      void setViewport(int width, int height);

      /* for debugging */
      void print(int fields);
};

#endif /* SCENE_H */
