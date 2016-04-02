#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SceneLib.h"

void interpolateNewVertex(Vertex* v_outside, Vertex* v_inside, Vertex* v_new, int eqn_number);
void interpolateNewTriangle(Vertex* v0_in, Vertex* v1_out, Vertex* v2_out, Triangle* tp, Scene* s, int);
void interpolateNewTriangles(Vertex* v0_out, Vertex* v1_in, Vertex* v2_in, Triangle* tp, Scene* s, int);

void P6_Clipping(Scene* scene)
{
   // Walk the list of Triangle objects and clip
   // the triangles so that all of their vertices
   // are within the clipping volume. Replace any
   // Triangle that sticks out of the clipping
   // volume with a set of Triangles that triangulate
   // the polygon that results when the sticking out
   // Triangle is clipped.

   TriangleListNode *ptr;  // use this pointer to walk the scene's list of triangles
   TriangleListNode *previous_ptr;

   previous_ptr = &(scene->head_node);

   for (ptr = (scene->head_node).next;  ptr;  ptr = ptr->next)
   {
      Triangle* tp = ptr->t;
      double w0 = fabs( tp->v[0].w );
      double w1 = fabs( tp->v[1].w );
      double w2 = fabs( tp->v[2].w );
      if ( fabs( tp->v[0].x ) > w0
        || fabs( tp->v[0].y ) > w0
        || fabs( tp->v[0].z ) > w0
        || fabs( tp->v[1].x ) > w1
        || fabs( tp->v[1].y ) > w1
        || fabs( tp->v[1].z ) > w1
        || fabs( tp->v[2].x ) > w2
        || fabs( tp->v[2].y ) > w2
        || fabs( tp->v[2].z ) > w2 )
      {// need to either delete or clip this triangle
         double w0 = tp->v[0].w;
         double w1 = tp->v[1].w;
         double w2 = tp->v[2].w;
         // first check for trivial delete
         if ( (w0 + tp->v[0].x < 0 && w1 + tp->v[1].x < 0 && w2 + tp->v[2].x < 0)
           || (w0 - tp->v[0].x < 0 && w1 - tp->v[1].x < 0 && w2 - tp->v[2].x < 0)
           || (w0 + tp->v[0].y < 0 && w1 + tp->v[1].y < 0 && w2 + tp->v[2].y < 0)
           || (w0 - tp->v[0].y < 0 && w1 - tp->v[1].y < 0 && w2 - tp->v[2].y < 0)
           || (w0 + tp->v[0].z < 0 && w1 + tp->v[1].z < 0 && w2 + tp->v[2].z < 0)
           || (w0 - tp->v[0].z < 0 && w1 - tp->v[1].z < 0 && w2 - tp->v[2].z < 0) )
         {/* trivial delete; the triangle will be deleted after this if-statement */
//fprintf(stderr, "trivial delete\n");
//fflush(stderr);
         }// not trivial delete; the triangle must get clipped
         else if (w0 + tp->v[0].x < 0 || w1 + tp->v[1].x < 0 || w2 + tp->v[2].x < 0)
         {// clip to the x = -1 plane
//fprintf(stderr, "clip to x = -1\n");
//fflush(stderr);
            if (w0 + tp->v[0].x < 0 && w1 + tp->v[1].x < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 1 );
            }
            else if (w0 + tp->v[0].x < 0 && w2 + tp->v[2].x < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 1 );
            }
            else if (w1 + tp->v[1].x < 0 && w2 + tp->v[2].x < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 1 );
            }
            else if (w0 + tp->v[0].x < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 1 );
            }
            else if (w1 + tp->v[1].x < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 1 );
            }
            else if (w2 + tp->v[2].x < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 1 );
            }
            else  // should never get here
            {
               fprintf(stderr, "clipping to x = -1 dropped a triangle!\n");
               fflush(stderr);
            }
         }
         else if (w0 - tp->v[0].x < 0 || w1 - tp->v[1].x < 0 || w2 - tp->v[2].x < 0)
         {// clip to the x = +1 plane
//fprintf(stderr, "clip to x = +1\n");
//fflush(stderr);
            if (w0 - tp->v[0].x < 0 && w1 - tp->v[1].x < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 2 );
            }
            else if (w0 - tp->v[0].x < 0 && w2 - tp->v[2].x < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 2 );
            }
            else if (w1 - tp->v[1].x < 0 && w2 - tp->v[2].x < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 2 );
            }
            else if (w0 - tp->v[0].x < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 2 );
            }
            else if (w1 - tp->v[1].x < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 2 );
            }
            else if (w2 - tp->v[2].x < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 2 );
            }
            else  // should never get here
            {
               fprintf(stderr, "clipping to x = +1 dropped a triangle!\n");
               fflush(stderr);
            }
         }
         else if (w0 + tp->v[0].y < 0 || w1 + tp->v[1].y < 0 || w2 + tp->v[2].y < 0)
         {// clip to the y = -1 plane
//fprintf(stderr, "clip to y = -1\n");
//fflush(stderr);
            if (w0 + tp->v[0].y < 0 && w1 + tp->v[1].y < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 3 );
            }
            else if (w0 + tp->v[0].y < 0 && w2 + tp->v[2].y < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 3 );
            }
            else if (w1 + tp->v[1].y < 0 && w2 + tp->v[2].y < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 3 );
            }
            else if (w0 + tp->v[0].y < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 3 );
            }
            else if (w1 + tp->v[1].y < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 3 );
            }
            else if (w2 + tp->v[2].y < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 3 );
            }
            else  // should never get here
            {
               fprintf(stderr, "clipping to y = -1 dropped a triangle!\n");
               fflush(stderr);
            }
         }
         else if (w0 - tp->v[0].y < 0 || w1 - tp->v[1].y < 0 || w2 - tp->v[2].y < 0)
         {// clip to the y = +1 plane
//fprintf(stderr, "clip to y = +1\n");
//fflush(stderr);
            if (w0 - tp->v[0].y < 0 && w1 - tp->v[1].y < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 4 );
            }
            else if (w0 - tp->v[0].y < 0 && w2 - tp->v[2].y < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 4 );
            }
            else if (w1 - tp->v[1].y < 0 && w2 - tp->v[2].y < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 4 );
            }
            else if (w0 - tp->v[0].y < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 4 );
            }
            else if (w1 - tp->v[1].y < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 4 );
            }
            else if (w2 - tp->v[2].y < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 4 );
            }
            else  // should never get here
            {
               fprintf(stderr, "clipping to y = +1 dropped a triangle!\n");
               fflush(stderr);
            }
         }
         else if (w0 + tp->v[0].z < 0 || w1 + tp->v[1].z < 0 || w2 + tp->v[2].z < 0)
         {// clip to the z = -1 plane
//fprintf(stderr, "clip to z = -1\n");
//fflush(stderr);
            if (w0 + tp->v[0].z < 0 && w1 + tp->v[1].z < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 5 );
            }
            else if (w0 + tp->v[0].z < 0 && w2 + tp->v[2].z < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 5 );
            }
            else if (w1 + tp->v[1].z < 0 && w2 + tp->v[2].z < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 5 );
            }
            else if (w0 + tp->v[0].z < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 5 );
            }
            else if (w1 + tp->v[1].z < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 5 );
            }
            else if (w2 + tp->v[2].z < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 5 );
            }
            else  // should never get here
            {
               fprintf(stderr, "clipping to z = -1 dropped a triangle!\n");
               fflush(stderr);
            }
         }
         else if (w0 - tp->v[0].z < 0 || w1 - tp->v[1].z < 0 || w2 - tp->v[2].z < 0)
         {// clip to the z = +1 plane
//fprintf(stderr, "clip to z = +1\n");
//fflush(stderr);
            if (w0 - tp->v[0].z < 0 && w1 - tp->v[1].z < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 6 );
            }
            else if (w0 - tp->v[0].z < 0 && w2 - tp->v[2].z < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 6 );
            }
            else if (w1 - tp->v[1].z < 0 && w2 - tp->v[2].z < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 6 );
            }
            else if (w0 - tp->v[0].z < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, scene, 6 );
            }
            else if (w1 - tp->v[1].z < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, scene, 6 );
            }
            else if (w2 - tp->v[2].z < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, scene, 6 );
            }
            else  // should never get here
            {
               fprintf(stderr, "clipping to z = +1 dropped a triangle!\n");
               fflush(stderr);
            }
         }
         else
         {// should never get here
            fprintf(stderr, "Clipping error!.\n");
            fflush(stderr);
            exit(-1);
         }

         // remove old Triangle from the linked list
         if (ptr == scene->tail_ptr) scene->tail_ptr = previous_ptr;
         previous_ptr->next = ptr->next;  // unlink the TriangleListNode object
         delete ptr->t;   // delete the Triangle object
         delete ptr;      // delete the TriangleListNode object
         ptr = previous_ptr;
      }
      previous_ptr = ptr;
   }// continue with the next triangle

   return;
}


void interpolateNewVertex(Vertex* v_outside, Vertex* v_inside, Vertex* v_new, int eqn_number)
{
//fprintf(stderr, "create new vertex\n");
//fflush(stderr);
   // make local copies of several values
   double vix =  v_inside->x; // "i" for "inside"
   double viy =  v_inside->y;
   double viz =  v_inside->z;
   double viw =  v_inside->w;
   double vox = v_outside->x; // "o" for "outside"
   double voy = v_outside->y;
   double voz = v_outside->z;
   double vow = v_outside->w;
   // interpolate between v_outside and v_inside
   double t;
   if (eqn_number == 1)
      t = -(vow + vox)/( (viw + vix) - (vow + vox) );
   else if (eqn_number == 2)
      t = -(vow - vox)/( (viw - vix) - (vow - vox) );
   else if (eqn_number == 3)
      t = -(vow + voy)/( (viw + viy) - (vow + voy) );
   else if (eqn_number == 4)
      t = -(vow - voy)/( (viw - viy) - (vow - voy) );
   else if (eqn_number == 5)
      t = -(vow + voz)/( (viw + viz) - (vow + voz) );
   else if (eqn_number == 6)
      t = -(vow - voz)/( (viw - viz) - (vow - voz) );

   t = t + .00001;  /* keep the new vertex off the edge!! */

   // interpolate the coordinates of the new vertex
   v_new->x = (1-t) * vox + t * vix;
   v_new->y = (1-t) * voy + t * viy;
   v_new->z = (1-t) * voz + t * viz;
   v_new->w = (1-t) * vow + t * viw;
   // interpolate the color and texture coordinates of the new vertex
   v_new->r = (1-t) * v_outside->r + t * v_inside->r;
   v_new->g = (1-t) * v_outside->g + t * v_inside->g;
   v_new->b = (1-t) * v_outside->b + t * v_inside->b;
   v_new->a = (1-t) * v_outside->a + t * v_inside->a;
   v_new->s = (1-t) * v_outside->s + t * v_inside->s;
   v_new->t = (1-t) * v_outside->t + t * v_inside->t;
   // interpolate the normal vector of the new vertex
   v_new->n = (v_outside->n).times(1-t).plus( (v_inside->n).times(t) );

//fprintf(stderr,"[% .6f % .6f % .6f % .6f]\n", vox, voy, voz, vow);
//fprintf(stderr, "t = % .6f\n", t);
//fprintf(stderr,"<% .6f % .6f % .6f % .6f>\n",  v_new->x,  v_new->y,  v_new->z,  v_new->w);
//fflush(stderr);

   return;
}


void interpolateNewTriangle(Vertex * v0_inside, Vertex * v1_outside, Vertex * v2_outside,
                            Triangle* tp, Scene* scene, int eqn_number)
{
//fprintf(stderr, "create one new triangle\n");
//fflush(stderr);
   // create two new vertices
   Vertex v1_new, v2_new;

   // interpolate a new vertex between v1_outside and v0_inside
   interpolateNewVertex(v1_outside, v0_inside, &v1_new, eqn_number);

   // interpolate a new vertex between v2_outside and v0_inside
   interpolateNewVertex(v2_outside, v0_inside, &v2_new, eqn_number);

   // create a new Triangle
   Triangle * t_new = new Triangle( v0_inside, &v1_new, &v2_new );
   t_new->tex  = tp->tex;
   t_new->mtrl = tp->mtrl;

   // add the new Triangle to the end of the linked list
   scene->addTriangle(t_new);

//t_new->print();

   return;
}


void interpolateNewTriangles(Vertex * v0_outside, Vertex * v1_inside, Vertex * v2_inside,
                             Triangle* tp, Scene* scene, int eqn_number)
{
//fprintf(stderr, "create two new triangles\n");
//fflush(stderr);
//scene->print();

   // create a new vertex
   Vertex v0_new1;

   // interpolate a new vertex between v0_outside and v1_inside
   interpolateNewVertex(v0_outside, v1_inside, &v0_new1, eqn_number);

   // create a new Triangle
   Triangle *t1_new = new Triangle( &v0_new1, v1_inside, v2_inside );
   t1_new->tex  = tp->tex;
   t1_new->mtrl = tp->mtrl;

   // create another new vertex
   Vertex v0_new2;

   // interpolate a new vertex between v0_outside and v2_inside
   interpolateNewVertex(v0_outside, v2_inside, &v0_new2, eqn_number);

   // create another new Triangle
   Triangle *t2_new = new Triangle( &v0_new1, &v0_new2, v2_inside );
   t2_new->tex  = tp->tex;
   t2_new->mtrl = tp->mtrl;

   // add the new Triangles to the end of the linked list
   scene->addTriangle(t1_new);
   scene->addTriangle(t2_new);

//t1_new->print();
//t2_new->print();
//scene->print();

   return;
}