#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include "Vertex.h"

class Matrix
{
   private:
   public:
      Vector v1, v2, v3, v4;  /* these are column vectors */

      Matrix();

      Matrix(Vector v);   /* translation matrix */

      Matrix(float d);    /* diagonal matrix */

      Matrix(float x, float y, float z);  /* scalling matrix */

      Matrix(float theta, float x, float y, float z); /* rotation matrix */

      Matrix(Vector _v1, Vector _v2, Vector _v3, Vector _v4);

      void setColumn(int col, float x, float y, float z, float w);

      Matrix times(float s);  /* scalar times Matrix */

      Vertex times(Vertex v);  /* Matrix times Vertex */

      Vector times(Vector v);  /* Matrix times Vector */

      Matrix times(Matrix m);  /* Matrix times Matrix */

      /* for debugging */
      void print();
};

#endif  /* MATRIX_H */
