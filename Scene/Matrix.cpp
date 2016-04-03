#include <stdio.h>
#include <math.h>
#include "Matrix.h"

#define PI 3.1415926535897932384626433832795


Matrix::Matrix()
{
   v1 = Vector();
   v2 = Vector();
   v3 = Vector();
   v4 = Vector();
}


Matrix::Matrix(Vector v)   /* translation matrix */
{
   v1 = Vector( 1.0, 0.0, 0.0, 0.0 );
   v2 = Vector( 0.0, 1.0, 0.0, 0.0 );
   v3 = Vector( 0.0, 0.0, 1.0, 0.0 );
   v4 = Vector( v.x, v.y, v.z, 1.0 );
}


Matrix::Matrix(double d)  /* diagonal matrix */
{
   v1 = Vector(   d, 0.0, 0.0, 0.0 );
   v2 = Vector( 0.0,   d, 0.0, 0.0 );
   v3 = Vector( 0.0, 0.0,   d, 0.0 );
   v4 = Vector( 0.0, 0.0, 0.0, 1.0 );
}


Matrix::Matrix(double x, double y, double z)  /* scaling matrix */
{
   v1 = Vector(   x, 0.0, 0.0, 0.0 );
   v2 = Vector( 0.0,   y, 0.0, 0.0 );
   v3 = Vector( 0.0, 0.0,   z, 0.0 );
   v4 = Vector( 0.0, 0.0, 0.0, 1.0 );
}


Matrix::Matrix(double theta, double x, double y, double z) /* rotation matrix */
{
   /*  http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml  */
   double norm = sqrt(x*x + y*y + z*z);
   double ux = x/norm;
   double uy = y/norm;
   double uz = z/norm;

   double c = cos( (PI/180.0)*theta );
   double s = sin( (PI/180.0)*theta );

   v1 = Vector( ux*ux*(1-c)+c,      uy*ux*(1-c)+(uz*s), uz*ux*(1-c)-(uy*s), 0.0 );
   v2 = Vector( ux*uy*(1-c)-(uz*s), uy*uy*(1-c)+c,      uz*uy*(1-c)+(ux*s), 0.0 );
   v3 = Vector( ux*uz*(1-c)+(uy*s), uy*uz*(1-c)-(ux*s), uz*uz*(1-c)+c,      0.0 );
   v4 = Vector( 0.0,                0.0,                0.0,                1.0 );
}


Matrix::Matrix(Vector _v1, Vector _v2, Vector _v3, Vector _v4)
{
   v1 = _v1;
   v2 = _v2;
   v3 = _v3;
   v4 = _v4;
}


void Matrix::setColumn(int col, double x, double y, double z, double w)
{
   if (col == 1)
   {
      v1.set(x, y, z, w);
   }
   else if (col == 2)
   {
      v2.set(x, y, z, w);
   }
   else if (col == 3)
   {
      v3.set(x, y, z, w);
   }
   else if (col == 4)
   {
      v4.set(x, y, z, w);
   }
}


Matrix Matrix::times(double s)  /* scalar times Matrix */
{
   return Matrix( v1.times(s), v2.times(s), v3.times(s), v4.times(s));
}


Matrix Matrix::times(Matrix m)  /* Matrix times Matrix */
{
   return Matrix( times(m.v1), times(m.v2), times(m.v3), times(m.v4) );
}


Vector Matrix::times(Vector v) /* Matrix times Vector */
{
   return v1.times(v.x).plus(v2.times(v.y).plus(v3.times(v.z).plus(v4.times(v.w))));
/*
   Vector v1x = v1.times(v.x);
   Vector v2y = v2.times(v.y);
   Vector v3z = v3.times(v.z);
   Vector v4w = v4.times(v.w);
   Vector sum1 = v1x.plus(v2y);
   Vector sum2 = sum1.plus(v3z);
   Vector sum3 = sum2.plus(v4w);
   return sum3;
*/
}


Vertex Matrix::times(Vertex v) /* Matrix times Vertex */
{
   Vector sum = v1.times(v.x).plus(v2.times(v.y).plus(v3.times(v.z).plus(v4.times(v.w))));
   Vertex temp = Vertex(sum.x, sum.y, sum.z, sum.w);
   // copy data fileds from the original vetex to the new vertwx
   temp.r = v.r;
   temp.g = v.g;
   temp.b = v.b;
   temp.a = v.a;
   temp.s = v.s;
   temp.t = v.t;
   temp.n = v.n;
   return temp;
}


/* for debugging */
void Matrix::print()
{
   fprintf(stderr,"[[% .6f % .6f % .6f % .6f]\n",  v1.x, v2.x, v3.x, v4.x);
   fprintf(stderr," [% .6f % .6f % .6f % .6f]\n",  v1.y, v2.y, v3.y, v4.y);
   fprintf(stderr," [% .6f % .6f % .6f % .6f]\n",  v1.z, v2.z, v3.z, v4.z);
   fprintf(stderr," [% .6f % .6f % .6f % .6f]]\n", v1.w, v2.w, v3.w, v4.w);
   fflush(stderr);
   return;
}
