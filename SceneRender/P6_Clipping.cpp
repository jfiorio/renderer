#include "P6_Clipping.h"
#include <new>

P6_Clipping::P6_Clipping() : PipelineStage()
{
  // This stage reads vertex positions
  // and modifies the arrangement of triangles
  attributes |= (STAGE_ATTRIB_READ_VERTEX_POSITION | STAGE_ATTRIB_WRITE_TRIANGLE_LIST);
}

void P6_Clipping::processTriangles(Context *context)
{
   // Walk the list of Triangle objects and clip
   // the triangles so that all of their vertices
   // are within the clipping volume. Replace any
   // Triangle that sticks out of the clipping
   // volume with a set of Triangles that triangulate
   // the polygon that results when the sticking out
   // Triangle is clipped.

   List<Triangle> *triangles = context->triangles;
   Pool<Triangle> *trianglePool = context->trianglePool;
   ListNode<Triangle> *prev = &triangles->sentinel;

   for (ListNode<Triangle> *ptr = triangles->head; ptr; ptr = ptr->next)
   {
      Triangle* tp = ptr;
      ptr->trianglePool = trianglePool;
      float w0 = fabs( tp->v[0].w );
      float w1 = fabs( tp->v[1].w );
      float w2 = fabs( tp->v[2].w );
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
         float w0 = tp->v[0].w;
         float w1 = tp->v[1].w;
         float w2 = tp->v[2].w;
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
            if (w0 + tp->v[0].x < 0 && w1 + tp->v[1].x < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 1 );
            }
            else if (w0 + tp->v[0].x < 0 && w2 + tp->v[2].x < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 1 );
            }
            else if (w1 + tp->v[1].x < 0 && w2 + tp->v[2].x < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 1 );
            }
            else if (w0 + tp->v[0].x < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 1 );
            }
            else if (w1 + tp->v[1].x < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 1 );
            }
            else if (w2 + tp->v[2].x < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 1 );
            }
            else  // should never get here
            {
               fprintf(stderr, "clipping to x = -1 dropped a triangle!\n");
               fflush(stderr);
            }
         }
         else if (w0 - tp->v[0].x < 0 || w1 - tp->v[1].x < 0 || w2 - tp->v[2].x < 0)
         {// clip to the x = +1 plane
            if (w0 - tp->v[0].x < 0 && w1 - tp->v[1].x < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 2 );
            }
            else if (w0 - tp->v[0].x < 0 && w2 - tp->v[2].x < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 2 );
            }
            else if (w1 - tp->v[1].x < 0 && w2 - tp->v[2].x < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 2 );
            }
            else if (w0 - tp->v[0].x < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 2 );
            }
            else if (w1 - tp->v[1].x < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 2 );
            }
            else if (w2 - tp->v[2].x < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 2 );
            }
            else  // should never get here
            {
               fprintf(stderr, "clipping to x = +1 dropped a triangle!\n");
               fflush(stderr);
            }
         }
         else if (w0 + tp->v[0].y < 0 || w1 + tp->v[1].y < 0 || w2 + tp->v[2].y < 0)
         {// clip to the y = -1 plane
            if (w0 + tp->v[0].y < 0 && w1 + tp->v[1].y < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 3 );
            }
            else if (w0 + tp->v[0].y < 0 && w2 + tp->v[2].y < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 3 );
            }
            else if (w1 + tp->v[1].y < 0 && w2 + tp->v[2].y < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 3 );
            }
            else if (w0 + tp->v[0].y < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 3 );
            }
            else if (w1 + tp->v[1].y < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 3 );
            }
            else if (w2 + tp->v[2].y < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 3 );
            }
            else  // should never get here
            {
               fprintf(stderr, "clipping to y = -1 dropped a triangle!\n");
               fflush(stderr);
            }
         }
         else if (w0 - tp->v[0].y < 0 || w1 - tp->v[1].y < 0 || w2 - tp->v[2].y < 0)
         {// clip to the y = +1 plane
            if (w0 - tp->v[0].y < 0 && w1 - tp->v[1].y < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 4 );
            }
            else if (w0 - tp->v[0].y < 0 && w2 - tp->v[2].y < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 4 );
            }
            else if (w1 - tp->v[1].y < 0 && w2 - tp->v[2].y < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 4 );
            }
            else if (w0 - tp->v[0].y < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 4 );
            }
            else if (w1 - tp->v[1].y < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 4 );
            }
            else if (w2 - tp->v[2].y < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 4 );
            }
            else  // should never get here
            {
               fprintf(stderr, "clipping to y = +1 dropped a triangle!\n");
               fflush(stderr);
            }
         }
         else if (w0 + tp->v[0].z < 0 || w1 + tp->v[1].z < 0 || w2 + tp->v[2].z < 0)
         {// clip to the z = -1 plane
            if (w0 + tp->v[0].z < 0 && w1 + tp->v[1].z < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 5 );
            }
            else if (w0 + tp->v[0].z < 0 && w2 + tp->v[2].z < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 5 );
            }
            else if (w1 + tp->v[1].z < 0 && w2 + tp->v[2].z < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 5 );
            }
            else if (w0 + tp->v[0].z < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 5 );
            }
            else if (w1 + tp->v[1].z < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 5 );
            }
            else if (w2 + tp->v[2].z < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 5 );
            }
            else  // should never get here
            {
               fprintf(stderr, "clipping to z = -1 dropped a triangle!\n");
               fflush(stderr);
            }
         }
         else if (w0 - tp->v[0].z < 0 || w1 - tp->v[1].z < 0 || w2 - tp->v[2].z < 0)
         {// clip to the z = +1 plane
            if (w0 - tp->v[0].z < 0 && w1 - tp->v[1].z < 0)
            {// create one new Triangle containing previous v[2] and new v[0] and v[1]
               interpolateNewTriangle( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 6 );
            }
            else if (w0 - tp->v[0].z < 0 && w2 - tp->v[2].z < 0)
            {// create one new Triangle containing previous v[1] and new v[0] and v[2]
               interpolateNewTriangle( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 6 );
            }
            else if (w1 - tp->v[1].z < 0 && w2 - tp->v[2].z < 0)
            {// create one new Triangle containing previous v[0] and new v[1] and v[2]
               interpolateNewTriangle( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 6 );
            }
            else if (w0 - tp->v[0].z < 0)
            {// create two new Triangles, niether containing previous v[0]
               interpolateNewTriangles( &(tp->v[0]), &(tp->v[1]), &(tp->v[2]), tp, context, 6 );
            }
            else if (w1 - tp->v[1].z < 0)
            {// create two new Triangles, niether containing previous v[1]
               interpolateNewTriangles( &(tp->v[1]), &(tp->v[2]), &(tp->v[0]), tp, context, 6 );
            }
            else if (w2 - tp->v[2].z < 0)
            {// create two new Triangles, niether containing previous v[2]
               interpolateNewTriangles( &(tp->v[2]), &(tp->v[0]), &(tp->v[1]), tp, context, 6 );
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
         triangles->remove(prev, ptr);
         trianglePool->reclaimNode(ptr);
         ptr = prev;
      }
      prev = ptr;
   }// continue with the next triangle

   return;
}


void interpolateNewVertex(Vertex* v_outside, Vertex* v_inside, Vertex* v_new, int eqn_number)
{
   // make local copies of several values
   float vix =  v_inside->x; // "i" for "inside"
   float viy =  v_inside->y;
   float viz =  v_inside->z;
   float viw =  v_inside->w;
   float vox = v_outside->x; // "o" for "outside"
   float voy = v_outside->y;
   float voz = v_outside->z;
   float vow = v_outside->w;
   // interpolate between v_outside and v_inside
   float t = 0.0;
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

   t = t + .00001f;  /* keep the new vertex off the edge!! */

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

   return;
}


void interpolateNewTriangle(Vertex * v0_inside, Vertex * v1_outside, Vertex * v2_outside,
                            Triangle* tp, Context *context, int eqn_number)
{
   // create two new vertices
   Vertex v1_new, v2_new;

   // interpolate a new vertex between v1_outside and v0_inside
   interpolateNewVertex(v1_outside, v0_inside, &v1_new, eqn_number);

   // interpolate a new vertex between v2_outside and v0_inside
   interpolateNewVertex(v2_outside, v0_inside, &v2_new, eqn_number);

   // allocate space for and create a new triangle
   Triangle *t_new = context->trianglePool->allocateNode();
   t_new = new ((void*)t_new) Triangle( v0_inside, &v1_new, &v2_new ); // placement new
   t_new->tex  = tp->tex;
   t_new->mtrl = tp->mtrl;
   
   // place the triangle at the end of the triangle list
   context->triangles->add(t_new);

   return;
}

void interpolateNewTriangles(Vertex * v0_outside, Vertex * v1_inside, Vertex * v2_inside,
                             Triangle* tp, Context *context, int eqn_number)
{
   // allocate space for 2 new triangles 
   Triangle *t1_new = context->trianglePool->allocateNode();
   Triangle *t2_new = context->trianglePool->allocateNode();
   
   // create a new vertex
   Vertex v0_new1;

   // interpolate a new vertex between v0_outside and v1_inside
   interpolateNewVertex(v0_outside, v1_inside, &v0_new1, eqn_number);

   // create the first Triangle
   t1_new = new ((void*)t1_new) Triangle( &v0_new1, v1_inside, v2_inside );
   t1_new->tex  = tp->tex;
   t1_new->mtrl = tp->mtrl;

   // create another new vertex
   Vertex v0_new2;

   // interpolate a new vertex between v0_outside and v2_inside
   interpolateNewVertex(v0_outside, v2_inside, &v0_new2, eqn_number);

   // create the second Triangle
   t2_new = new ((void*)t2_new) Triangle( &v0_new1, &v0_new2, v2_inside );
   t2_new->tex  = tp->tex;
   t2_new->mtrl = tp->mtrl;
   
   // place the triangles at the end of the triangle list
   context->triangles->add(t1_new);
   context->triangles->add(t2_new);
   
   return;
}