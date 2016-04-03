#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vertex.h"
#include "Texture.h"
#include "Material.h"
#include "Fragment.h"
#include "List.h"

class Triangle
{
   private:
   public:
      Vertex v[3];    /* the vertices of this triangle */
      Texture *tex;   /* point to a texture data structure */
      Material *mtrl; /* point to a data structure that describes material properties */
      
      List<Fragment> fragments; /* list of fragments (after rasterization stage) */
      Pool<Fragment> *fragmentPool; /* pool fragments allocated from */
      Pool<Triangle> *trianglePool; /* pool of triangles allocated from */
      
      Triangle();
      ~Triangle();  // ~Triangle() calls ~Fragment().
                    // The Material object must be deleted manually!!

      Triangle(Vertex * v0, Vertex * v1, Vertex * v2);

      void setTexture(Texture * t);

      /* for debugging */
      void print(int fields);
};

#endif  /* TRIANGLE_H */
