#ifndef VERTEX_H
#define VERTEX_H

#include "Vector.h"

class Vertex
{
   private:
   public:
      double x, y, z, w;  /* a vertex in homogenous coordinates */
      double r, g, b, a;  /* color at this vertex (including transparency) */
      double s, t;        /* texture coordinates at this vertex */
      Vector n;           /* normal vector at this vertex */

      Vertex();

      Vertex(double x, double y, double z);

      Vertex(double x, double y, double z, double w);

      Vertex(float v[4]);

      void set(double x, double y, double z);

      void set(double _x, double _y, double _z, double _w);

      /* sets the color and transparancy of this vertex to (r,g,b,a) */
      void setColor(double _r, double _g, double _b, double _a);

      void setColor(double _r, double _g, double _b);

      void setColor(int _r, int _g, int _b, int _a);

      void setColor(int _r, int _g, int _b);

      /* sets the texture coordinate of this vertex to (s, t) */
      void setCoords(double _s, double _t);

      void setNormal(Vector * _n);

      Vertex times(double scalar);

      /* for debugging */
      void print(int fields);
      void print();
};

#endif  /* VERTEX_H */
