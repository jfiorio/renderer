#include "P10_FragmentShaderTexture.h"

P10_FragmentShaderTexture::P10_FragmentShaderTexture() : PipelineStage()
{
  attributes |= (STAGE_ATTRIB_READ_VERTEX_POSITION | 
                 STAGE_ATTRIB_READ_FRAGMENT_LIST | 
                 STAGE_ATTRIB_WRITE_VERTEX_COLOR);  

  for (int i = 0; i < 256; i++)
    colors[i] = i/255.0;
}

void P10_FragmentShaderTexture::processTriangles(Context *context)
{
  for (ListNode<Triangle> *ptr = context->triangles->head; ptr; ptr = ptr->next)
  {
    if (NULL == ptr->tex)  // if this triangle does not have a texture
    {
      // For this triangle, get the color information
      // from each of the three vertices.
      double r0 = ptr->v[0].r;
      double g0 = ptr->v[0].g;
      double b0 = ptr->v[0].b;
      double a0 = ptr->v[0].a;
      double r1 = ptr->v[1].r;
      double g1 = ptr->v[1].g;
      double b1 = ptr->v[1].b;
      double a1 = ptr->v[1].a;
      double r2 = ptr->v[2].r;
      double g2 = ptr->v[2].g;
      double b2 = ptr->v[2].b;
      double a2 = ptr->v[2].a;
   
      // Walk this triangle's list of fragments.
      for (ListNode<Fragment> *fptr = ptr->fragments.head; fptr; fptr = fptr->next)
      {
        // get this fragment's barycentric coordinates
        double alpha = fptr->alpha;
        double beta  = fptr->beta;
        double gamma = fptr->gamma;
   
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
    
      ALIGN16 __m256d temp1, temp2, temp3, temp4;
      ALIGN16 __m256d tempr1[4], tempr2[4], tempr3[4];
      
      // For this triangle, get the texture coordinates
      // from each of the three vertices.
      double s0 = ptr->v[0].s;
      double t0 = ptr->v[0].t;
      double s1 = ptr->v[1].s;
      double t1 = ptr->v[1].t;
      double s2 = ptr->v[2].s;
      double t2 = ptr->v[2].t;
	  	
      // Also get the w coordinates, 
      // for perspective-correct mapping
      double w0 = ptr->v[0].w;
      double w1 = ptr->v[1].w;
      double w2 = ptr->v[2].w;
      
      // Get width, height, texture data pointer
      int w = ptr->tex->width;
	  	int h = ptr->tex->height;
      unsigned int *data = (unsigned int*)ptr->tex->data;
      
      ALIGN16 __m256d wv = { w, w, w, w };
      ALIGN16 __m256d hv = { h, h, h, h };
      ALIGN16 __m256i swap_2x1 = _mm256_set_epi64x(0x0, 0x2, 0x0, 0x2);
            
      // Set up texture coordinate vectors
      ALIGN16 __m256d scoords = _mm256_setr_pd(s0, s1, s2, 0);
      ALIGN16 __m256d tcoords = _mm256_setr_pd(t0, t1, t2, 0);
      
      // Set up w coordinate vector 
      // for perspective correct mapping
      ALIGN16 __m256d wcoords = _mm256_setr_pd(w0, w1, w2, 0);

      ALIGN16 __m256i mask = { -1, -1, 0, 0 };
      
      ListNode<Fragment> *fragment;
      if (ptr->fragments.size == 0) 
      { 
      fragment = 0; 
      }
      else 
      { 
      fragment = ptr->fragments.head->next; 
      }
      Fragment *fragmentr[4];
      int c=0;
      while (fragment)
      {
        int rc;
        for (rc=0; rc<4; rc++)
        {
          fragmentr[rc] = fragment;
          
          // get this fragment's barycentric coordinates
          ALIGN16 __m256d coords =  _mm256_loadu_pd((double*)&fragment->alpha);
          
          // interpolate texture coordinates, for this fragment, from the triangle's vertices
          temp3 = _mm256_div_pd(coords, wcoords);
          temp1 = _mm256_mul_pd(temp3, scoords);
          temp2 = _mm256_mul_pd(temp3, tcoords);
          temp4 = _mm256_setr_pd(temp1[2], temp2[2], temp3[2], 0);
          temp1 = _mm256_permute2f128_pd(temp1, temp3, 0x20);
          temp1 = _mm256_hadd_pd(temp1, temp2);
          temp1 = _mm256_add_pd(temp1, temp4);
          temp3 = _mm256_set1_pd(temp1[2]);
          tempr1[rc] = _mm256_div_pd(temp1,temp3);
          
          fragment = fragment->next;
          if (!fragment) { break; }
        }
        for (int r=0,s=0; r<2; r++,s+=2)
        {
          tempr2[r] = _mm256_setr_pd(tempr1[s][0], tempr1[s][1], tempr1[s+1][0], tempr1[s+1][1]);
          tempr3[r] = _mm256_floor_pd(tempr2[r]);          // wrap the texture in both the s
          tempr3[r] = _mm256_sub_pd(tempr2[r], tempr3[r]); // and t coordinate directions
        }
        temp2 = _mm256_setr_pd(tempr3[0][1], tempr3[0][3], tempr3[1][1], tempr3[1][3]);
        temp2 = _mm256_mul_pd(temp2, hv);
        temp2 = _mm256_floor_pd(temp2);
        temp1 = _mm256_setr_pd(tempr3[0][0], tempr3[0][2], tempr3[1][0], tempr3[1][2]);
        temp2 = _mm256_add_pd(temp1, temp2);
        temp1 = _mm256_mul_pd(temp2, wv);
 
        for (int r=0; r<rc; r++)
        {
          unsigned int offset = temp1[r];
          // unsigned int offset = (((int)(t*h)+s)*w);
          fragmentr[r]->pixel = data[offset];
        }
      }
    }
  }
}

// double s = temp1[0];
// double t = temp1[1];
// temp2 = _mm256_permutevar_pd(temp1, swap_2x1);
// temp2 = _mm256_mul_pd(temp2, hv);
// temp2 = _mm256_floor_pd(temp2);
// temp2 = _mm256_add_pd(temp2, temp1);
// temp1 = _mm256_mul_pd(temp2, wv);

// tempr1[0] = { s_ur1, t_ur1, 0, 0 }
// tempr1[1] = { s_ur2, t_ur2, 0, 0 }
// tempr1[2] = { s_ur3, t_ur3, 0, 0 }
// tempr1[3] = { s_ur4, t_ur4, 0, 0 }

// tempr2[0] = { s_ur1, t_ur1, s_ur2, t_ur2 }
// tempr2[1] = { s_ur3, t_ur3, s_ur4, t_ur4 }

// tempr3[0] = { s1, t1, s2, t2 }
// tempr3[1] = { s3, t3, s4, t4 }