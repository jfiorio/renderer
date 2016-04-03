#include "P4_Lighting.h"

P4_Lighting::P4_Lighting() : PipelineStage()
{
  attributes |= (STAGE_ATTRIB_READ_VERTEX_POSITION | STAGE_ATTRIB_WRITE_VERTEX_COLOR);  
}

void P4_Lighting::processTriangles(Context *context)
{
  // Walk the list of Triangle objects.
  // For each Vertex in each Triangle object,
  // use the Scene's light data, the Triangle's
  // material properties, and the Vertex's normal
  // vector to compute color information for that
  // vertex.
  
  Scene *scene = context->scene;
  List<Triangle> *triangles = context->triangles;
  if (scene->doLighting && scene->light)
  {
    if (scene->doSmoothLighting)
    {
      doSmoothLighting(scene, triangles);
    }
    else if (scene->doOpenglFlatLighting != 0)
    {
      doSmoothLighting(scene, triangles);
    }
    else
    {
      doFlatLighting(scene, triangles);
    }
  }
  return;
}

void doSmoothLighting(Scene *scene, List<Triangle> *triangles)
{
   // Compute the transformation matrix that transforms normal
   // vectors from model coordinates into view coordinates.
   // Get the 3-by-3 part of the model matrix that we need.
   double m11 = scene->model.v1.x, m12 = scene->model.v2.x, m13 = scene->model.v3.x,
          m21 = scene->model.v1.y, m22 = scene->model.v2.y, m23 = scene->model.v3.y,
          m31 = scene->model.v1.z, m32 = scene->model.v2.z, m33 = scene->model.v3.z;
   // Get the 3-by-3 part of the view matrix that we need.
   double v11 = scene->view.v1.x, v12 = scene->view.v2.x, v13 = scene->view.v3.x,
          v21 = scene->view.v1.y, v22 = scene->view.v2.y, v23 = scene->view.v3.y,
          v31 = scene->view.v1.z, v32 = scene->view.v2.z, v33 = scene->view.v3.z;
   // Compute V*M, the modelview matrix.
   double mv11 = v11*m11 + v12*m21 + v13*m31,
          mv12 = v11*m12 + v12*m22 + v13*m32,
          mv13 = v11*m13 + v12*m23 + v13*m33,
          mv21 = v21*m11 + v22*m21 + v23*m31,
          mv22 = v21*m12 + v22*m22 + v23*m32,
          mv23 = v21*m13 + v22*m23 + v23*m33,
          mv31 = v31*m11 + v32*m21 + v33*m31,
          mv32 = v31*m12 + v32*m22 + v33*m32,
          mv33 = v31*m13 + v32*m23 + v33*m33;
   // Compute (V*M)^{-T}, the inverse transpose of the modelview matrix.
   // We are using the trick here of computing the inverse using cofactors,
   // but without dividing by the determinant. So this is not the true inverse,
   // but we only need our "inverse" to point our vectors in the correct direction.
   // We will then normalize our vectors to length 1.
   Vector col1 = Vector(mv22*mv33-mv23*mv32, mv13*mv32-mv12*mv33, mv12*mv23-mv13*mv22);
   Vector col2 = Vector(mv23*mv31-mv21*mv33, mv11*mv33-mv13*mv31, mv13*mv21-mv11*mv23);
   Vector col3 = Vector(mv21*mv32-mv22*mv31, mv12*mv31-mv11*mv32, mv11*mv22-mv12*mv21);
   Vector col4 = Vector(0.0, 0.0, 0.0, 1.0);
   Matrix MVInvTranspose = Matrix(col1, col2, col3, col4);

   for (ListNode<Triangle> *ptr = triangles->head; ptr && ptr->mtrl; ptr = ptr->next)
   {
      // Get all of the values that we need.
      double s = ptr->mtrl->s; //shininess coefficient
      double kar = ptr->mtrl->ambient[0];  // red
      double kdr = ptr->mtrl->diffuse[0];
      double ksr = ptr->mtrl->specular[0];
      double car = scene->light->ambient[0];
      double cdr = scene->light->diffuse[0];
      double csr = scene->light->specular[0];
      double kag = ptr->mtrl->ambient[1];  // green
      double kdg = ptr->mtrl->diffuse[1];
      double ksg = ptr->mtrl->specular[1];
      double cag = scene->light->ambient[1];
      double cdg = scene->light->diffuse[1];
      double csg = scene->light->specular[1];
      double kab = ptr->mtrl->ambient[2];  // blue
      double kdb = ptr->mtrl->diffuse[2];
      double ksb = ptr->mtrl->specular[2];
      double cab = scene->light->ambient[2];
      double cdb = scene->light->diffuse[2];
      double csb = scene->light->specular[2];

      // Do the calculation at vertex v[0].

      // To do vector algebra, we need the vertex as a vector.
      Vector p = Vector(ptr->v[0].x, ptr->v[0].y, ptr->v[0].z);
      // We need the normal vector at this vertex.
      // Here is this vertex's normal vector in model(!) coordinates.
      Vector N = ptr->v[0].n;
      // We need to convert this normal vector into view coordinates.
      N = MVInvTranspose.times( N );
      N = N.normalize();
      // Put this normal vector into the vertex (for debugging purposes).
      ptr->v[0].n = N;
      // We need the unit light vector, L, from our vertex to the light source.
      // Here is the light source as a vertex in world coordinates.
      Vertex S = Vertex(scene->light->pos);
      // Convert light source from world coordinates to view coordinates.
      Matrix viewMatrix = scene->view;
      S = viewMatrix.times(S);
      // Convert the vertex to a vector.
      Vector S2 = Vector(S.x, S.y, S.z);
      // Compute L = S2 - p, the vector from our vertex to the light source.
      Vector L = S2.plus( p.times(-1.0) );
      // normaliz L
      L = L.normalize();
      // We need the reflection vector R = 2(L dot N)*N - L.
      Vector R = N.times( 2*L.dotProduct(N) ).plus( L.times(-1.0) );
      // We need the unit view vector, V, that points from the vertex to the origin.
      Vector V = p.normalize().times(-1.0);
      // We can also use the "half vector", H = normalize(L + V).
      Vector H = L.plus(V).normalize();

      // Compute the color at this vertex using its normal, light, reflection, and view vectors,
      //    c = k_a * c_a + sum_{all lights}( k_d*c_d*(L dot N) + k_s*c_s*(R dot V)^s ) )
      // or, using the half vector H,
      //    c = k_a * c_a + sum_{all lights}( k_d*c_d*(L dot N) + k_s*c_s*(N dot H)^s ) )

      // precompute some numbers
      double diffuseAngle = L.dotProduct(N);
      diffuseAngle = (diffuseAngle < 0) ? 0.0 : diffuseAngle;
      double shininess;
      if (scene->useHalfVector)
      {
         shininess = N.dotProduct(H);
      }
      else
      {
         shininess = R.dotProduct(V);
      }
      shininess = (shininess < 0) ? 0.0 : pow(shininess, s);

      // do red
      double r = kar*car + kdr*cdr*diffuseAngle + ksr*csr*shininess;
      r = (r <= 1.0) ? r : 1.0;  // clamp the color value at 1.0
      ptr->v[0].r = r;

      // do green
      double g = kag*cag + kdg*cdg*diffuseAngle + ksg*csg*shininess;
      g = (g <= 1.0) ? g : 1.0;  // clamp the color value at 1.0
      ptr->v[0].g = g;

      // do blue
      double b = kab*cab + kdb*cdb*diffuseAngle + ksb*csb*shininess;
      b = (b <= 1.0) ? b : 1.0;  // clamp the color value at 1.0
      ptr->v[0].b = b;

      if ( !(scene->doSmoothLighting) && (scene->doOpenglFlatLighting==1) )
      {  /* Do OpenGL style flat lighting. */
         /* Copy the lighting information from vertex v[0]
            to the other two vertices, v[1] and v[2]. */
         ptr->v[1].r = r;
         ptr->v[1].g = g;
         ptr->v[1].b = b;
         ptr->v[2].r = r;
         ptr->v[2].g = g;
         ptr->v[2].b = b;
         continue; /* ugly, ugly, ugly */
      }

      // Do the calculation at vertex v[1].

      // To do vector algebra, we need the vertex as a vector.
      p = Vector(ptr->v[1].x, ptr->v[1].y, ptr->v[1].z);
      // We need the normal vector at this vertex.
      // Here is this vertex's normal vector in model(!) coordinates.
      N = ptr->v[1].n;
      // We need to convert this normal vector into view coordinates.
      N = MVInvTranspose.times( N );
      N = N.normalize();
      // Put this normal vector into the vertex (for debugging purposes).
      ptr->v[1].n = N;
      // We need the unit light vector, L, from our vertex to the light source.
      // Compute L = S2 - p, the vector from our vertex to the light source.
      L = S2.plus( p.times(-1.0) );
      // normaliz L
      L = L.normalize();
      // We need the reflection vector R = 2(L dot N)*N - L.
      R = N.times( 2*L.dotProduct(N) ).plus( L.times(-1.0) );
      // We need the unit view vector, V, that points from the vertex to the origin.
      V = p.normalize().times(-1.0);
      // We can also use the "half vector", H = normalize(L + V).
      H = L.plus(V).normalize();

      // Compute the color at this vertex using its normal, light, reflection, and view vectors,
      //    c = k_a * c_a + sum_{all lights}( k_d*c_d*(L dot N) + k_s*c_s*(R dot V)^s ) )
      // or, using the half vector H,
      //    c = k_a * c_a + sum_{all lights}( k_d*c_d*(L dot N) + k_s*c_s*(N dot H)^s ) )

      // precompute some numbers
      diffuseAngle = L.dotProduct(N);
      diffuseAngle = (diffuseAngle < 0) ? 0.0 : diffuseAngle;
      if (scene->useHalfVector)
      {
         shininess = N.dotProduct(H);
      }
      else
      {
         shininess = R.dotProduct(V);
      }
      shininess = (shininess < 0) ? 0.0 : pow(shininess, s);

      // do red
      r = kar*car + kdr*cdr*diffuseAngle + ksr*csr*shininess;
      r = (r <= 1.0) ? r : 1.0;  // clamp the color value at 1.0
      ptr->v[1].r = r;

      // do green
      g = kag*cag + kdg*cdg*diffuseAngle + ksg*csg*shininess;
      g = (g <= 1.0) ? g : 1.0;  // clamp the color value at 1.0
      ptr->v[1].g = g;

      // do blue
      b = kab*cab + kdb*cdb*diffuseAngle + ksb*csb*shininess;
      b = (b <= 1.0) ? b : 1.0;  // clamp the color value at 1.0
      ptr->v[1].b = b;

      if ( !(scene->doSmoothLighting) && (scene->doOpenglFlatLighting==2) )
      {  /* Do OpenGL style flat lighting. */
         /* Copy the lighting information from vertex v[1]
            to the other two vertices, v[0] and v[2]. */
         ptr->v[0].r = r;
         ptr->v[0].g = g;
         ptr->v[0].b = b;
         ptr->v[2].r = r;
         ptr->v[2].g = g;
         ptr->v[2].b = b;
         continue; /* ugly, ugly, ugly */
      }

      // Do the calculation at vertex v[2].

      // To do vector algebra, we need the vertex as a vector.
      p = Vector(ptr->v[2].x, ptr->v[2].y, ptr->v[2].z);
      // We need the normal vector at this vertex.
      // Here is this vertex's normal vector in model(!) coordinates.
      N = ptr->v[2].n;
      // We need to convert this normal vector into view coordinates.
      N = MVInvTranspose.times( N );
      N = N.normalize();
      // Put this normal vector into the vertex (for debugging purposes).
      ptr->v[2].n = N;
      // We need the unit light vector, L, from our vertex to the light source.
      // Compute L = S2 - p, the vector from our vertex to the light source.
      L = S2.plus( p.times(-1.0) );
      // normaliz L
      L = L.normalize();
      // We need the reflection vector R = 2(L dot N)*N - L.
      R = N.times( 2*L.dotProduct(N) ).plus( L.times(-1.0) );
      // We need the unit view vector, V, that points from the vertex to the origin.
      V = p.normalize().times(-1.0);
      // We can also use the "half vector", H = normalize(L + V).
      H = L.plus(V).normalize();

      // Compute the color at this vertex using its normal, light, reflection, and view vectors,
      //    c = k_a * c_a + sum_{all lights}( k_d*c_d*(L dot N) + k_s*c_s*(R dot V)^s ) )
      // or, using the half vector H,
      //    c = k_a * c_a + sum_{all lights}( k_d*c_d*(L dot N) + k_s*c_s*(N dot H)^s ) )

      // precompute some numbers
      diffuseAngle = L.dotProduct(N);
      diffuseAngle = (diffuseAngle < 0) ? 0.0 : diffuseAngle;
      if (scene->useHalfVector)
      {
         shininess = N.dotProduct(H);
      }
      else
      {
         shininess = R.dotProduct(V);
      }
      shininess = (shininess < 0) ? 0.0 : pow(shininess, s);

      // do red
      r = kar*car + kdr*cdr*diffuseAngle + ksr*csr*shininess;
      r = (r <= 1.0) ? r : 1.0;  // clamp the color value at 1.0
      ptr->v[2].r = r;

      // do green
      g = kag*cag + kdg*cdg*diffuseAngle + ksg*csg*shininess;
      g = (g <= 1.0) ? g : 1.0;  // clamp the color value at 1.0
      ptr->v[2].g = g;

      // do blue
      b = kab*cab + kdb*cdb*diffuseAngle + ksb*csb*shininess;
      b = (b <= 1.0) ? b : 1.0;  // clamp the color value at 1.0
      ptr->v[2].b = b;

      if ( !(scene->doSmoothLighting) && (scene->doOpenglFlatLighting==3) )
      {  /* Do OpenGL style flat lighting. */
         /* Copy the lighting information from vertex v[2]
            to the other two vertices, v[0] and v[1]. */
         ptr->v[0].r = r;
         ptr->v[0].g = g;
         ptr->v[0].b = b;
         ptr->v[1].r = r;
         ptr->v[1].g = g;
         ptr->v[1].b = b;
         //continue; /* ugly, ugly, ugly */
      }
   }
   return;
}


void doFlatLighting(Scene *scene, List<Triangle> *triangles)
{
   for (ListNode<Triangle> *ptr = triangles->head; ptr && ptr->mtrl; ptr = ptr->next)
   {
      // Compute a normal vector, N, for this triangle. We need to be carefull
      // that the normal vector has the right "orientation". We want the normal
      // vector for a CCW triangle (which is the default for a "forward facing
      // triangle") to point towards the origin (in view coordinates). Compare
      // with P3_BackFaceCulling.cpp. Another way to put this is that for a
      // closed surface made up of CCW triangles, the normal vector should
      // point outside of the surface.
      Vector p1 = Vector(ptr->v[1].x - ptr->v[0].x,
                         ptr->v[1].y - ptr->v[0].y,
                         ptr->v[1].z - ptr->v[0].z);
      Vector p2 = Vector(ptr->v[2].x - ptr->v[0].x,
                         ptr->v[2].y - ptr->v[0].y,
                         ptr->v[2].z - ptr->v[0].z);
      Vector N = p1.crossProduct(p2);
      N = N.normalize();

      Vector p0 = Vector(ptr->v[0].x, ptr->v[0].y, ptr->v[0].z);

      // p = p0 + (v1-v0)/3 + (v2-v0)/3 is our center (position) vector for this triangle.
      Vector p = p0.plus( p1.times(1.0/3.0).plus(p2.times(1.0/3.0)) );

      // We need the unit light vector, L, from our center position to the light source.
      // Here is the light source as a vertex in world coordinates.
      Vertex S = Vertex(scene->light->pos);
      // Convert light source from world coordinates to view coordinates.
      Matrix m = scene->view;
      S = m.times(S);
      // Convert the vertex to a vector.
      Vector S2 = Vector(S.x, S.y, S.z);
      // Compute L = S2 - p, the vector from our center position to the light source.
      Vector L = S2.plus( p.times(-1.0) );
      // normaliz L
      L = L.normalize();

      // We need the reflection vector R = 2(L dot N)*N - L.
      Vector R = N.times( 2*L.dotProduct(N) ).plus( L.times(-1.0) );

      // We need the unit view vector, V, that points from the center position
      // to the origin (in view coordinates).
      Vector V = p.normalize().times(-1.0);

      // We can also use the "half vector", H = normalize(L + V).
      Vector H = L.plus(V).normalize();

//fprintf(stderr, "p = "); p.print();  // center point
//fprintf(stderr, "S = "); S.print();  // light source
//fprintf(stderr, "N = "); N.print();  // normal
//fprintf(stderr, "L = "); L.print();  // light
//fprintf(stderr, "R = "); R.print();  // reflection
//fprintf(stderr, "V = "); V.print();  // view
//fprintf(stderr, "H = "); H.print();  // half
//fprintf(stderr, "\n");
//fflush(stderr);

      // Compute the color at the center position using normal, light, reflection, and view vectors,
      //    c = k_a * c_a + sum_{all lights}( k_d*c_d*(L dot N) + k_s*c_s*(R dot V)^s ) )
      // or, using the half vector H.
      //    c = k_a * c_a + sum_{all lights}( k_d*c_d*(L dot N) + k_s*c_s*(N dot H)^s ) )

      // precompute some numbers
      double s = ptr->mtrl->s; //shininess coefficient
      double diffuseAngle = L.dotProduct(N);
      diffuseAngle = (diffuseAngle < 0) ? 0.0 : diffuseAngle;
      double shininess;
      if (scene->useHalfVector)
      {
         shininess = N.dotProduct(H);
      }
      else
      {
         shininess = R.dotProduct(V);
      }
      shininess = (shininess < 0) ? 0.0 : pow(shininess, s);

      // do red
      double kar = ptr->mtrl->ambient[0];
      double kdr = ptr->mtrl->diffuse[0];
      double ksr = ptr->mtrl->specular[0];
      double car = scene->light->ambient[0];
      double cdr = scene->light->diffuse[0];
      double csr = scene->light->specular[0];
      double r = kar*car + kdr*cdr*diffuseAngle + ksr*csr*shininess;
      r = (r <= 1.0) ? r : 1.0;  // clamp the color value at 1.0
      ptr->v[0].r = r;
      ptr->v[1].r = r;
      ptr->v[2].r = r;


      // do green
      double kag = ptr->mtrl->ambient[1];
      double kdg = ptr->mtrl->diffuse[1];
      double ksg = ptr->mtrl->specular[1];
      double cag = scene->light->ambient[1];
      double cdg = scene->light->diffuse[1];
      double csg = scene->light->specular[1];
      double g = kag*cag + kdg*cdg*diffuseAngle + ksg*csg*shininess;
      g = (g <= 1.0) ? g : 1.0;  // clamp the color value at 1.0
      ptr->v[0].g = g;
      ptr->v[1].g = g;
      ptr->v[2].g = g;

      // do blue
      double kab = ptr->mtrl->ambient[2];
      double kdb = ptr->mtrl->diffuse[2];
      double ksb = ptr->mtrl->specular[2];
      double cab = scene->light->ambient[2];
      double cdb = scene->light->diffuse[2];
      double csb = scene->light->specular[2];
      double b = kab*cab + kdb*cdb*diffuseAngle + ksb*csb*shininess;
      b = (b <= 1.0) ? b : 1.0;  // clamp the color value at 1.0
      ptr->v[0].b = b;
      ptr->v[1].b = b;
      ptr->v[2].b = b;
   }
   return;
}