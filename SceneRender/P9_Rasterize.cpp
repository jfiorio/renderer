#include "P9_Rasterize.h"

P9_Rasterize::P9_Rasterize() : PipelineStage()
{
  // This stage reads vertex positions
  // and modifies the arrangement of fragments
  attributes |= (STAGE_ATTRIB_READ_VERTEX_POSITION | STAGE_ATTRIB_WRITE_FRAGMENT_LIST);
}

ALIGN16 __m256d zero = _mm256_set1_pd(0); 
ALIGN16 __m256i swap_2x1 = _mm256_setr_epi64x(0x2, 0x0, 0x2, 0x0);
ALIGN16 __m256d mask_4d = _mm256_setr_pd(-0x1, -0x1, -0x1, 0x0);
ALIGN16 __m128i mask_4 = _mm_setr_epi32(-0x1, -0x1, -0x1, -0x1);

void P9_Rasterize::processTriangles(Context *context)
{    
   // Unfortunately we can't use floats for the iterative version of the 
   // triangle rasterization algorithm; otherwise there is a loss of precision 
   // in the calculations and we get random artifacts that we don't want.
   // Everything here is (mostly) the same as in the double version except 
   // for the [down]casting of alpha, beta, and gamma for storage.
   
   ALIGN16 __m256d one = _mm256_set1_pd(1);

   ALIGN16 avx_fpoint2 vertices[2][3], vert_diff_un[2][3], vert_ordered[2][2];
   ALIGN16 avx_fpoint2 vert_cross_t[2][3], vert_swap[2][3];
   ALIGN16 avx_fpoint2 vert_diff_xt[2][3], vert_diff_yt[2][3];
   ALIGN16 avx_fpoint2 alpha_num_t[2], gamma_num_t[2];
   ALIGN16 avx_fpoint2 gamma_denom_t[2];
   ALIGN16 avx_fpoint2 max[2], min[2];
   ALIGN16 avx_fpoint2 ymaxxmin[2], dim[2];

   ALIGN16 avx_fpoint2 vert_cross[3];
   ALIGN16 avx_fpoint2 alpha, beta, gamma;
   ALIGN16 avx_fpoint2 alpha_num, gamma_num;
   ALIGN16 avx_fpoint2 gamma_denom;
   
   ALIGN16 avx_fpoint2 vert_diff_rfact;
   ALIGN16 avx_fpoint2 vert_diff_x[3], vert_diff_y[3], vert_diff_r[3];
      
   Pool<Fragment> *fragmentPool = context->fragmentPool;
   ListNode<Triangle> *triangles[2][2]; 
   ListNode<Triangle> *ptr = context->triangles->head;
   while (ptr)
   { 
      // load vertices for the next 2x2 triangles
      // (note: loading done so as to favor the vertex difference operation)
      int c=0;
      triangles[0][0] = 0;
      triangles[0][1] = 0;
      triangles[1][0] = 0;
      triangles[1][1] = 0;
      for (int t=0; (t<2) && ptr; t++)
      {
        for (int i=0; (i<2) && ptr; i++)
        {
          vertices[t][0].values_ji[i].xj = ptr->v[1].x; 
          vertices[t][0].values_ji[i].yi = ptr->v[0].y;
          vertices[t][1].values_ij[i].xi = ptr->v[0].x;
          vertices[t][1].values_ij[i].yj = ptr->v[1].y;
          vertices[t][2].values[i].x = ptr->v[2].x;
          vertices[t][2].values[i].y = ptr->v[2].y;
          triangles[t][i] = ptr;
          ptr = ptr->next;
          c++;
        }  
      }

      // do calculations for next 2x2 triangles
      for (int t=0; t<2; t++)
      {
        // compute maxs and mins for each of the next 2 triangles
        max[t].data = _mm256_max_pd(vertices[t][0].data, vertices[t][1].data);
        max[t].data = _mm256_max_pd(max[t].data, vertices[t][2].data);
        min[t].data = _mm256_min_pd(vertices[t][0].data, vertices[t][1].data);
        min[t].data = _mm256_min_pd(min[t].data, vertices[t][2].data);
        max[t].data = _mm256_ceil_pd(max[t].data);
        min[t].data = _mm256_floor_pd(min[t].data);
      
        // compute vertex differences for each of the 2 triangles
        vert_diff_xt[t][0].data = _mm256_sub_pd(vertices[t][0].data, vertices[t][1].data);
        vert_diff_un[t][1].data = _mm256_sub_pd(vertices[t][2].data, vertices[t][0].data);
        vert_diff_un[t][2].data = _mm256_sub_pd(vertices[t][1].data, vertices[t][2].data);
 
        // permute for the correct order
        vert_diff_xt[t][1].data = _mm256_blend_pd(vert_diff_un[t][1].data, vert_diff_un[t][2].data, 0b1010);
        vert_diff_xt[t][2].data = _mm256_blend_pd(vert_diff_un[t][2].data, vert_diff_un[t][1].data, 0b1010);
      
        // generate ordered x0a, y0a, ... and x1a, y1a, ... permutations
        vert_ordered[t][0].data = _mm256_blend_pd(vertices[t][1].data, vertices[t][0].data, 0b1010);
        vert_ordered[t][1].data = _mm256_blend_pd(vertices[t][0].data, vertices[t][1].data, 0b1010);
        
        // swap adjacent pairs to generate permutations for computing cross products
        vert_swap[t][0].data = _mm256_permutevar_pd(vert_ordered[t][0].data, swap_2x1);
        vert_swap[t][1].data = _mm256_permutevar_pd(vert_ordered[t][1].data, swap_2x1);
        vert_swap[t][2].data = _mm256_permutevar_pd(vertices[t][2].data, swap_2x1);
                                                
        // multiplication portion of cross product
        vert_cross_t[t][0].data = _mm256_mul_pd(vert_diff_xt[t][0].data, vert_swap[t][0].data);
        vert_cross_t[t][1].data = _mm256_mul_pd(vert_diff_xt[t][1].data, vert_swap[t][1].data);
        
        // multiplication portion of denominators
        gamma_denom_t[t].data = _mm256_mul_pd(vert_diff_xt[t][0].data, vert_swap[t][2].data);

        // generate ymaxa, xmina, ... permutation 
        ymaxxmin[t].data = _mm256_blend_pd(min[t].data, max[t].data, 0b1010);
        ymaxxmin[t].data = _mm256_permutevar_pd(ymaxxmin[t].data, swap_2x1);
        
        // multiplication portion of initial alpha and gamma numerators
        gamma_num_t[t].data = _mm256_mul_pd(vert_diff_xt[t][0].data, ymaxxmin[t].data);
        alpha_num_t[t].data = _mm256_mul_pd(vert_diff_xt[t][1].data, ymaxxmin[t].data);

        // compute triangle bound box dimensions
        dim[t].data = _mm256_sub_pd(max[t].data, min[t].data);

        // generate permutation with y increment factors as even elements
        vert_diff_yt[t][0].data = _mm256_permutevar_pd(vert_diff_xt[t][0].data, swap_2x1);
        vert_diff_yt[t][1].data = _mm256_permutevar_pd(vert_diff_xt[t][1].data, swap_2x1);
        vert_diff_yt[t][2].data = _mm256_permutevar_pd(vert_diff_xt[t][2].data, swap_2x1);
      }
      
      // addition portion of cross product calculations
      vert_cross[0].data = _mm256_hadd_pd(vert_cross_t[0][0].data, vert_cross_t[1][0].data);
      vert_cross[1].data = _mm256_hadd_pd(vert_cross_t[0][1].data, vert_cross_t[1][1].data);

      // remaining denominator calculations
      gamma_denom.data = _mm256_hadd_pd(gamma_denom_t[0].data, gamma_denom_t[1].data);
      gamma_denom.data = _mm256_sub_pd(gamma_denom.data, vert_cross[0].data);
      
      // remaining alpha and gamma numerator calculations
      gamma_num.data = _mm256_hadd_pd(gamma_num_t[0].data, gamma_num_t[1].data);
      alpha_num.data = _mm256_hadd_pd(alpha_num_t[0].data, alpha_num_t[1].data);
      gamma_num.data = _mm256_sub_pd(gamma_num.data, vert_cross[0].data);
      alpha_num.data = _mm256_sub_pd(alpha_num.data, vert_cross[1].data);

      // compute initial alpha, beta, and gamma values
      alpha.data = _mm256_div_pd(alpha_num.data, gamma_denom.data);
      gamma.data = _mm256_div_pd(gamma_num.data, gamma_denom.data);
      beta.data = _mm256_sub_pd(one, gamma.data);
      beta.data = _mm256_sub_pd(beta.data, alpha.data);

      // blend even and odd elements of the diff_xt, diff_yt pairs, respectively, for x and y differences 
      vert_diff_x[0].data = _mm256_blend_pd(vert_diff_xt[0][0].data, vert_diff_yt[1][0].data, 0b1010);
      vert_diff_x[1].data = _mm256_blend_pd(vert_diff_xt[0][1].data, vert_diff_yt[1][1].data, 0b1010);
      vert_diff_x[2].data = _mm256_blend_pd(vert_diff_xt[0][2].data, vert_diff_yt[1][2].data, 0b1010);
      vert_diff_y[0].data = _mm256_blend_pd(vert_diff_yt[0][0].data, vert_diff_xt[1][0].data, 0b1010);
      vert_diff_y[1].data = _mm256_blend_pd(vert_diff_yt[0][1].data, vert_diff_xt[1][1].data, 0b1010);
      vert_diff_y[2].data = _mm256_blend_pd(vert_diff_yt[0][2].data, vert_diff_xt[1][2].data, 0b1010);
      
      // compute alpha, beta, and gamma increment/decrement factors
      vert_diff_x[0].data = _mm256_div_pd(vert_diff_x[0].data, gamma_denom.data);
      vert_diff_x[1].data = _mm256_div_pd(vert_diff_x[1].data, gamma_denom.data);
      vert_diff_x[2].data = _mm256_div_pd(vert_diff_x[2].data, gamma_denom.data);
      vert_diff_y[0].data = _mm256_div_pd(vert_diff_y[0].data, gamma_denom.data);
      vert_diff_y[1].data = _mm256_div_pd(vert_diff_y[1].data, gamma_denom.data);
      vert_diff_y[2].data = _mm256_div_pd(vert_diff_y[2].data, gamma_denom.data);     

      // compute and add y restoration factors to x decrement factors
      vert_diff_rfact.data = _mm256_set_pd(dim[1].data[2], dim[0].data[2], dim[1].data[0], dim[0].data[0]);

      vert_diff_r[0].data = _mm256_mul_pd(vert_diff_y[0].data, vert_diff_rfact.data);
      vert_diff_r[1].data = _mm256_mul_pd(vert_diff_y[1].data, vert_diff_rfact.data);
      vert_diff_r[2].data = _mm256_mul_pd(vert_diff_y[2].data, vert_diff_rfact.data);
      vert_diff_x[0].data = _mm256_add_pd(vert_diff_x[0].data, vert_diff_r[0].data);
      vert_diff_x[1].data = _mm256_add_pd(vert_diff_x[1].data, vert_diff_r[1].data);
      vert_diff_x[2].data = _mm256_add_pd(vert_diff_x[2].data, vert_diff_r[2].data);
      
      int ip[4] = { 0, 1, 0, 1 };
      int tp[4] = { 0, 0, 1, 1 };
      
      for (int d=0; d<4; d++)
      {
        int i = ip[d];
        int t = tp[d];
        int m = t << 1;
        
        ListNode<Triangle> *tri = triangles[i][t];
        if (!tri) { continue; }
        int x1 = min[i].data[m];
        int x2 = max[i].data[m];
        int y1 = min[i].data[m+1];
        int y2 = max[i].data[m+1];
        
        double a = alpha.data[d];
        double b = beta.data[d];
        double g = gamma.data[d];
        double gx = vert_diff_x[0].data[d];
        double ax = vert_diff_x[1].data[d];
        double bx = vert_diff_x[2].data[d];
        double gy = vert_diff_y[0].data[d];
        double ay = vert_diff_y[1].data[d];          
        double by = vert_diff_y[2].data[d];  

        double z0 = tri->v[0].z;
        double z1 = tri->v[1].z;
        double z2 = tri->v[2].z;
        
        __m128 abg32;
        __m256d abg, abgx, abgy, zcoords;
        __m256d z[3], zb[3];
        abg = z[0] = _mm256_setr_pd(a, b, g, 0);
        abgx = z[1] = _mm256_setr_pd(ax, bx, gx, 0);
        abgy = z[2] = _mm256_setr_pd(ay, by, gy, 0);
        zcoords = _mm256_setr_pd(z0, z1, z2, 0);
        for (int j=0; j<3; j++)
        {
          z[j] = _mm256_mul_pd(z[j], zcoords);
          zb[j] = _mm256_permute2f128_pd(z[j], z[j], 0x01);
          z[j] = _mm256_hadd_pd(z[j], zb[j]);
          z[j] = _mm256_hadd_pd(z[j], z[j]);
        }
        abg = _mm256_blend_pd(abg, z[0], 0b1000);
        abgx = _mm256_blend_pd(abgx, z[1], 0b1000);
        abgy = _mm256_blend_pd(abgy, z[2], 0b1000);
                  
        int findex = 0;
        ListNode<Fragment> *fragments = fragmentPool->beginAllocate();
        ListNode<Fragment> *f = fragments;
        for (int y=y2; y>y1; y--)
        {
          for (int x=x1; x<x2; x++)
          {
            if (_mm256_testz_pd(abg, mask_4d))
            {
              f = f->next;
              findex++;
              f->xIndex = x;
              f->yIndex = y;
              abg32 = _mm256_cvtpd_ps(abg);
              _mm_maskstore_ps(&f->alpha, mask_4, abg32);
            } 
            abg = _mm256_add_pd(abg, abgy);
          }
          abg = _mm256_sub_pd(abg, abgx);
        }
        fragmentPool->endAllocate(f, findex, &tri->fragments);
        tri->fragmentPool = fragmentPool;
        if (c==d) { return; }
      }
   }        
   return;
}
