#include <stdio.h>
#include <math.h>
#include "Vector.h"

Vector::Vector()
{
   x = 0.0;
   y = 0.0;
   z = 0.0;
   w = 0.0;
}


Vector::Vector(float _x, float _y, float _z)
{
   set(_x, _y, _z, 0.0);
}


Vector::Vector(float _x, float _y, float _z, float _w)
{
   set(_x, _y, _z, _w);
}


void Vector::set(float _x, float _y, float _z, float _w)
{
   x = _x;
   y = _y;
   z = _z;
   w = _w;
}


Vector Vector::times(float s) /* scalar times Vector */
{
   return Vector(s*x, s*y, s*z, s*w);
}


Vector Vector::plus(Vector v) /* Vector addition */
{
   return Vector( x+(v.x), y+(v.y), z+(v.z), w+(v.w) );
}

Vector Vector::crossProduct(Vector v)
{
   return Vector((y*v.z)-(z*v.y), (z*v.x)-(x*v.z), (x*v.y)-(y*v.x));
}

float Vector::dotProduct(Vector v)
{
   return x*v.x + y*v.y + z*v.z;
}

Vector Vector::normalize()  /* return a vector with length 1 */
{
   float norm = sqrt( x*x + y*y + z*z );
   return Vector(x/norm, y/norm, z/norm);
}


/* for debugging */
void Vector::print()
{
   fprintf(stderr,"[x,y,z,w] = [% .6f % .6f % .6f % .6f]\n", x, y, z, w);
   fflush(stderr);
}
