#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
   private:
   public:
      float x, y, z, w; /* a vector in homogenous coordinates */

      Vector();

      Vector(float x, float y, float z);

      Vector(float x, float y, float z, float w);

      void set(float _x, float _y, float _z, float _w);

      Vector times(float s); /* scalar times Vector */

      Vector plus(Vector v);  /* Vector addition */

      Vector crossProduct(Vector v);

      float dotProduct(Vector v);

      Vector normalize();     /* return a vector with length 1 */

      /* for debugging */
      void print();
};

#endif  /* VECTOR_H */
