#include <stdio.h>
#include <stdlib.h>
#include "Vertex.h"

Vertex::Vertex()
{
   set(0.0, 0.0, 0.0, 1.0);

   setColor(0.0, 0.0, 0.0, 0.0);

   s = 0.0;
   t = 0.0;

   n = Vector();
}


Vertex::Vertex(double _x, double _y, double _z)
{
   set(_x, _y, _z, 1.0);

   setColor(0.0, 0.0, 0.0, 0.0);

   s = 0.0;
   t = 0.0;

   n = Vector();
}


Vertex::Vertex(double _x, double _y, double _z, double _w)
{
   set(_x, _y, _z, _w);

   setColor(0.0, 0.0, 0.0, 0.0);

   s = 0.0;
   t = 0.0;

   n = Vector();
}


Vertex::Vertex(float v[4])
{
   set(v[0], v[1], v[2], v[3]);
}


void Vertex::set(double _x, double _y, double _z)
{
   set(_x, _y, _z, 1.0);
}


void Vertex::set(double _x, double _y, double _z, double _w)
{
   x = _x;
   y = _y;
   z = _z;
   w = _w;
}


/* sets the color and transparancy of this vertex to (r,g,b,a) */
void Vertex::setColor(double _r, double _g, double _b, double _a)
{
   if ( (_r < 0.0) || (_r > 1.0)
     || (_g < 0.0) || (_g > 1.0)
     || (_b < 0.0) || (_b > 1.0)
     || (_a < 0.0) || (_a > 1.0) )
   {
      fprintf(stderr,"ERROR! Invalid double color for vertex\n");
      r = _r;
      g = _g;
      b = _b;
      a = _a;
      print(3);
      fflush(stderr);
      exit(-1);
   }

   r = _r;
   g = _g;
   b = _b;
   a = _a;

   return;
}


void Vertex::setColor(double _r, double _g, double _b)
{
   setColor(_r, _g, _b, 0.0);
   return;
}


void Vertex::setColor(int _r, int _g, int _b, int _a)
{
   if ( (_r < 0) || (_r > 255)
     || (_g < 0) || (_g > 255)
     || (_b < 0) || (_b > 255)
     || (_a < 0) || (_a > 255))
   {
      fprintf(stderr,"ERROR! Invalid int color for vertex\n");
      r = ((float)_r)/(float)255;
      g = ((float)_g)/(float)255;
      b = ((float)_b)/(float)255;
      a = ((float)_a)/(float)255;
      print(3);
      fflush(stderr);
      exit(-1);
   }

   r = ((float)_r)/(float)255;
   g = ((float)_g)/(float)255;
   b = ((float)_b)/(float)255;
   a = ((float)_a)/(float)255;
   return;
}


void Vertex::setColor(int _r, int _g, int _b)
{
   setColor(_r, _g, _b, 0);
   return;
}


/* sets the texture coordinate of this vertex to (s, t) */
void Vertex::setCoords(double _s, double _t)
{
   s = _s;
   t = _t;
   return;
}


void Vertex::setNormal(Vector * _n)
{
   n = *(_n);
   return;
}


/* needed for perspective division step in the rendering pipeline */
Vertex Vertex::times(double scalar) /* scalar times Vertex */
{
   Vertex temp = Vertex(scalar*x, scalar*y, scalar*z, scalar*w);
   temp.r = r;
   temp.g = g;
   temp.b = b;
   temp.a = a;
   temp.s = s;
   temp.t = t;
   temp.n = n;
   return temp;
}


/* for debugging */
void Vertex::print(int fields)
{
   if (fields & 1)
   {
      fprintf(stderr,"[x,y,z,w] = [% .6f % .6f % .6f % .6f]\n", x, y, z, w);
   }
   if (fields & 2)
   {
      fprintf(stderr,"<r,g,b,a> = <% .6f % .6f % .6f % .6f>\n", r, g, b, a);
   }
   if (fields & 4)
   {
      fprintf(stderr,"{s,t} = {% .6f % .6f}\n", s, t);
   }
   if (fields & 8)
   {
      fprintf(stderr,"n -> ");
      n.print();
   }
   fflush(stderr);
   return;
}

void Vertex::print()
{
   fprintf(stderr,"[x,y,z,w] = [% .6f % .6f % .6f % .6f]\n", x, y, z, w);
   fflush(stderr);
   return;
}
