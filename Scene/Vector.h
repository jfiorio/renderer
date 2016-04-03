#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
   private:
   public:
      double x, y, z, w; /* a vector in homogenous coordinates */

      Vector();

      Vector(double x, double y, double z);

      Vector(double x, double y, double z, double w);

      void set(double _x, double _y, double _z, double _w);

      Vector times(double s); /* scalar times Vector */

      Vector plus(Vector v);  /* Vector addition */

      Vector crossProduct(Vector v);

      double dotProduct(Vector v);

      Vector normalize();     /* return a vector with length 1 */

      /* for debugging */
      void print();
};

#endif  /* VECTOR_H */
