#ifndef VERTEX_H
#define VERTEX_H

#include "Vector.h"

class Vertex
{
   private:
   public:
      float x, y, z, w;  /* a vertex in homogenous coordinates */
      float r, g, b, a;  /* color at this vertex (including transparency) */
      float s, t;        /* texture coordinates at this vertex */
      Vector n;           /* normal vector at this vertex */

      Vertex();

      Vertex(float x, float y, float z);

      Vertex(float x, float y, float z, float w);

      Vertex(float v[4]);

      void set(float x, float y, float z);

      void set(float _x, float _y, float _z, float _w);

      /* sets the color and transparancy of this vertex to (r,g,b,a) */
      void setColor(float _r, float _g, float _b, float _a);

      void setColor(float _r, float _g, float _b);

      void setColor(int _r, int _g, int _b, int _a);

      void setColor(int _r, int _g, int _b);

      /* sets the texture coordinate of this vertex to (s, t) */
      void setCoords(float _s, float _t);

      void setNormal(Vector * _n);

      Vertex times(float scalar);

      /* for debugging */
      void print(int fields);
      void print();
};

#endif  /* VERTEX_H */
