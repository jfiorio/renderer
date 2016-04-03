#include "P10_FragmentShaderTexture.h"

P10_FragmentShaderTexture::P10_FragmentShaderTexture() : PipelineStage()
{
  attributes |= (STAGE_ATTRIB_READ_VERTEX_POSITION | 
                 STAGE_ATTRIB_READ_FRAGMENT_LIST | 
                 STAGE_ATTRIB_WRITE_VERTEX_COLOR);  
}

void P10_FragmentShaderTexture::processTriangles(Context *context)
{
  for (ListNode<Triangle> *ptr = context->triangles->head; ptr; ptr = ptr->next)
  {
    if (NULL == ptr->tex)  // if this triangle does not have a texture
    {
      // For this triangle, get the color information
      // from each of the three vertices.
      float r0 = ptr->v[0].r;
      float g0 = ptr->v[0].g;
      float b0 = ptr->v[0].b;
      //float a0 = ptr->v[0].a;
      float r1 = ptr->v[1].r;
      float g1 = ptr->v[1].g;
      float b1 = ptr->v[1].b;
      //float a1 = ptr->v[1].a;
      float r2 = ptr->v[2].r;
      float g2 = ptr->v[2].g;
      float b2 = ptr->v[2].b;
      //float a2 = ptr->v[2].a;
   
      // Walk this triangle's list of fragments.
      for (ListNode<Fragment> *fptr = ptr->fragments.head; fptr; fptr = fptr->next)
      {
        // get this fragment's barycentric coordinates
        float alpha = fptr->alpha;
        float beta  = fptr->beta;
        float gamma = fptr->gamma;
   
        //fprintf(stderr, "alpha = %f, beta = %f, gamma = %f\n", alpha,beta,gamma);
   
        // interpolate color data from the triangle's vertices to this fragment
        fptr->r = (alpha*r0 + beta*r1 + gamma*r2)*255;
        fptr->g = (alpha*g0 + beta*g1 + gamma*g2)*255;
        fptr->b = (alpha*b0 + beta*b1 + gamma*b2)*255;
        //fptr->a = alpha*a0 + beta*a1 + gamma*a2;
        //fprintf(stderr, "r = %f, g = %f, b = %f\n", fptr->r,fptr->g,fptr->b);
      }
    }
    else  // do the texture case
    {     
      #ifndef ALIGN16
      #define ALIGN16 __attribute__ ((aligned(32)))
      #endif
    
      ALIGN16 __m128 coords[8] = { 0 };
      ALIGN16 __m256 temp0, temp1, temp2, temp3, temp4;

      // For this triangle, get the texture coordinates
      // from each of the three vertices.
      float s0 = ptr->v[0].s;
      float t0 = ptr->v[0].t;
      float s1 = ptr->v[1].s;
      float t1 = ptr->v[1].t;
      float s2 = ptr->v[2].s;
      float t2 = ptr->v[2].t;
	  	
      // Also get the w coordinates, 
      // for perspective-correct mapping
      float w0 = ptr->v[0].w;
      float w1 = ptr->v[1].w;
      float w2 = ptr->v[2].w;
      
      // Get width, height, texture data pointer
      int w = ptr->tex->width;
	  	int h = ptr->tex->height;
      unsigned int *data = (unsigned int*)ptr->tex->data;
      
      ALIGN16 __m256 wv = _mm256_set1_ps(w);
      ALIGN16 __m256 hv = _mm256_set1_ps(h);
            
      // Set up texture coordinate vectors
      ALIGN16 __m256 s0coords = _mm256_set1_ps(s0);
      ALIGN16 __m256 s1coords = _mm256_set1_ps(s1);
      ALIGN16 __m256 s2coords = _mm256_set1_ps(s2);

      ALIGN16 __m256 t0coords = _mm256_set1_ps(t0);
      ALIGN16 __m256 t1coords = _mm256_set1_ps(t1);
      ALIGN16 __m256 t2coords = _mm256_set1_ps(t2);

      // Set up w coordinate vector 
      // for perspective correct mapping
      ALIGN16 __m256 w0coords = _mm256_set1_ps((float)1/w0);
      ALIGN16 __m256 w1coords = _mm256_set1_ps((float)1/w1);
      ALIGN16 __m256 w2coords = _mm256_set1_ps((float)1/w2);

      ListNode<Fragment> *fragment = ptr->fragments.size ? ptr->fragments.head->next : 0;

      Fragment *fragmentr[8] = { 0 };
      while (fragment)
      {        
        int f;
        for (f=0; f<8; f++)
        {
          fragmentr[f] = fragment;
          if (!fragmentr[f]) { break; }
          coords[f] = _mm_loadu_ps((float*)&fragment->alpha);
          fragment = fragment->next;
        }
        
        temp0 = _mm256_setr_ps(coords[0][0], coords[1][0], coords[2][0], coords[3][0],
                               coords[4][0], coords[5][0], coords[6][0], coords[7][0]);
        temp1 = _mm256_setr_ps(coords[0][1], coords[1][1], coords[2][1], coords[3][1],
                               coords[4][1], coords[5][1], coords[6][1], coords[7][1]);
        temp2 = _mm256_setr_ps(coords[0][2], coords[1][2], coords[2][2], coords[3][2],
                               coords[4][2], coords[5][2], coords[6][2], coords[7][2]);
                   
        temp0 = _mm256_mul_ps(temp0, w0coords); // a/w0
        temp1 = _mm256_mul_ps(temp1, w1coords); // b/w1
        temp2 = _mm256_mul_ps(temp2, w2coords); // g/w2
        
        temp3 = _mm256_mul_ps(temp0, s0coords); // s0*(a/w0)
        temp4 = _mm256_mul_ps(temp1, s1coords); // s1*(b/w1)
        
        temp3 = _mm256_add_ps(temp3, temp4);    // s0*(a/w0)+s1*(b/w1)
        temp4 = _mm256_mul_ps(temp2, s2coords); // s2*(g/w2)
        temp3 = _mm256_add_ps(temp3, temp4);    // s0*(a/w0)+s1*(b/w1)+s2*(g/w2)
        
        temp4 = _mm256_mul_ps(temp0, t0coords); // t0*(a/w0)
        temp0 = _mm256_add_ps(temp0, temp1);    // a/w0 + b/w1
        temp0 = _mm256_add_ps(temp0, temp2);    // a/w0 + b/w1 + g/w2
        
        temp1 = _mm256_mul_ps(temp1, t1coords); // t1*(b/w1)
        temp1 = _mm256_add_ps(temp1, temp4);    // t1*(b/w1)+t0*(a/w0)
        temp2 = _mm256_mul_ps(temp2, t2coords); // t2*(b/w2)
        temp1 = _mm256_add_ps(temp1, temp2);    // t0*(a/w0)+t1*(b/w1)+t2*(g/w2)
        
        temp3 = _mm256_div_ps(temp3, temp0);    // s = (s0*(a/w0)+s1*(b/w1)+s2*(g/w2))/(a/w0 + b/w1 + g/w2)
        temp1 = _mm256_div_ps(temp1, temp0);    // t = (t0*(a/w0)+t1*(b/w1)+t2*(g/w2))/(a/w0 + b/w1 + g/w2)
        
        temp0 = _mm256_floor_ps(temp3);
        temp0 = _mm256_sub_ps(temp3, temp0);    // s - (int)s
        temp2 = _mm256_floor_ps(temp1);
        temp1 = _mm256_sub_ps(temp1, temp2);    // t - (int)t
        
        temp1 = _mm256_mul_ps(temp1, hv);
        temp1 = _mm256_floor_ps(temp1);
        temp0 = _mm256_add_ps(temp0, temp1);
        temp0 = _mm256_mul_ps(temp0, wv);

        for (int r=0; r<f; r++)
        {
          unsigned int offset = temp0[r];
          fragmentr[r]->pixel = data[offset];
        }
      }
    }
  }
}