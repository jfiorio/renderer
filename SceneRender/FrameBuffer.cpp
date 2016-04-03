/*
   This frame buffer represents a two-dimensional array of pixel data
   stored as a one dimensional array in row-major order. The first
   row of pixels should be displayed as the top of the image
   that is made up of this pixel data.

   The "viewport" is a two-dimensional sub array of the frame buffer.
   The viewport is represented by its upper left hand corner and
   lower right hand corner.

   This frame buffer also includes a z-buffer for representing depth
   information.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FrameBuffer.h"

#include <immintrin.h>

FrameBuffer::FrameBuffer()
{
   width  = 0;
   height = 0;
   vp_ul_x = 0;
   vp_ul_y = 0;
   vp_lr_x = 0;
   vp_lr_y = 0;
   
   pixel_buffer = 0;
   z_buffer = 0;
}
 
FrameBuffer::FrameBuffer(int w, int h)
{
   width  = w;
   height = h;
   /* set the default viewport */
   vp_ul_x = 0;
   vp_ul_y = 0;
   vp_lr_x = width - 1;
   vp_lr_y = height - 1;

   /* allocate the row-major pixel buffer */
   pixel_buffer = (unsigned char *)malloc(sizeof(unsigned char) * width * height * 3);
   if ( ! pixel_buffer )
   {
      fprintf(stderr, "ERROR! Unable to allocate memory for frame buffer\n");
      fflush(stderr);
      exit(-1);
   }

   /* allocate the z-buffer (the depth buffer) */
   z_buffer = (double *)malloc(sizeof(double) * width * height);
   if ( ! z_buffer )
   {
      fprintf(stderr, "ERROR! Unable to allocate memory for z-buffer\n");
      fflush(stderr);
      exit(-1);
   }

   /* initialize the pixel and z buffers */
   clearFB(0, 0, 0);
}


FrameBuffer::~FrameBuffer()
{
   if ( pixel_buffer )
      free(pixel_buffer);

   if ( z_buffer )
      free(z_buffer);
}


int FrameBuffer::getWidthFB()
{
   return width;
}


int FrameBuffer::getHeightFB()
{
   return height;
}


/* clear the framebuffer (set a background color) and the z-buffer */
void FrameBuffer::clearFB(unsigned char r, unsigned char g, unsigned char b)
{
  
  int count = (height * width) / 32;

  __m256i rgbrg = _mm256_setr_epi8( r, g, b, r, g, b, r, g, b, r, g, b, r, g, b, r,
                                    g, b, r, g, b, r, g, b, r, g, b, r, g, b, r, g );
  __m256i brgbr = _mm256_setr_epi8( b, r, g, b, r, g, b, r, g, b, r, g, b, r, g, b,
                                    r, g, b, r, g, b, r, g, b, r, g, b, r, g, b, r );
  __m256i gbrgb = _mm256_setr_epi8( g, b, r, g, b, r, g, b, r, g, b, r, g, b, r, g,
                                    b, r, g, b, r, g, b, r, g, b, r, g, b, r, g, b );

  
  __m256i *ptr = (__m256i*)pixel_buffer;
  for (int i = 0; i < count; i++)
  {
    _mm256_storeu_si256(ptr++, rgbrg);
    _mm256_storeu_si256(ptr++, brgbr);
    _mm256_storeu_si256(ptr++, gbrgb);
  }
  // clear the z-buffer (the depth buffer)
  clearzFB();
}

__m256d one = _mm256_set_pd(1, 1, 1, 1);

/* clear the frambuffer's z-buffer (the depth buffer) */
void FrameBuffer::clearzFB()
{
  double *ptr = z_buffer;
  
  int count = (height*width) >> 2;
  for (int i=0; i<count; i++, ptr+=4)
    _mm256_store_pd(ptr, one);
   
}


void FrameBuffer::setViewport(int _vp_ul_x, int _vp_ul_y, int _vp_lr_x, int _vp_lr_y)
{
   vp_ul_x = _vp_ul_x;
   vp_ul_y = _vp_ul_y;
   vp_lr_x = _vp_lr_x;
   vp_lr_y = _vp_lr_y;
}


int FrameBuffer::getWidthVP()
{
   return vp_lr_x - vp_ul_x + 1;
}


int FrameBuffer::getHeightVP()
{
   return vp_lr_y - vp_ul_y + 1;
}


/* clear the viewport's color and z-buffer */
void FrameBuffer::clearVP(unsigned char r, unsigned char g, unsigned char b)
{
   for (int y = vp_ul_y; y <= vp_lr_y; y++)
   {
      for (int x = vp_ul_x; x <= vp_lr_x; x++)
      {
         unsigned char *index = pixel_buffer + (y*width + x)*3;
         *(index + 0) = r;
         *(index + 1) = g;
         *(index + 2) = b;
      }
   }
   // clear the viewport's z-buffer (the depth buffer)
   clearzVP();
}


/* clear the viewport's z-buffer (the depth buffer) */
void FrameBuffer::clearzVP()
{
   for (int y = vp_ul_y; y <= vp_lr_y; y++)
   {
      for (int x = vp_ul_x; x <= vp_lr_x; x++)
      {
         *(z_buffer + y*width + x) = 1;
      }
   }
}


/* get the color of the pixel with coordinates (x,y) in the framebuffer */
void FrameBuffer::getPixelFB(int x, int y, unsigned char **c)
{
   *c = pixel_buffer + (y*width + x)*3;
   return;
}


/* get the depth of the pixel with coordinates (x,y) in the framebuffer */
double FrameBuffer::getDepthFB(int x, int y)
{
   return *(z_buffer + y*width + x);
}


/* set the color and depth of the pixel with coordinates (x,y) in the framebuffer */
void FrameBuffer::setPixelFB(int x, int y, unsigned char *c, double z)
{
   unsigned char *p = pixel_buffer + (y*width + x)*3;
   
   p[0] = c[0];
   p[1] = c[1];
   p[2] = c[2];

   *(z_buffer + y*width + x) = z;
   return;
}


/* get the color of the pixel with coordinates (x,y) relative to the current viewport */
void FrameBuffer::getPixelVP(int x, int y, unsigned char **c)
{
   *c = pixel_buffer + ((vp_ul_y + y)*width + vp_ul_x + x)*3; /* ???????????????? */
   return;
}


/* get the depth of the pixel with coordinates (x,y) relative to the current viewport */
double FrameBuffer::getDepthVP(int x, int y)
{
   return *(z_buffer + (vp_ul_y + y)*width + vp_ul_x + x);  /* ???????????????? */
}




/* set the color and depth of the pixel with coordinates (x,y) relative to the current viewport */

//void FrameBuffer::setPixelVP(int x, int y, int pixel/*unsigned char *c*/, double z)
//{
//  int offset = (vp_ul_y + y)*width + vp_ul_x + x;
//   unsigned char *p = pixel_buffer + offset*3;
//   // p[0] = c[0];
//   // p[1] = c[1];
//   // p[2] = c[2];
//   *(unsigned int*)(p) = (pixel) | (p[3] << 24);
//   
//   //tmp = _mm_set1_pi8(128);
//   //_m_maskmovq(tmp, maskb, (char*)p);
//
//   *(z_buffer + offset) = z;  /* ????????????????????? */
//   return;
//}


/* write the framebuffer to the specified file. */
void FrameBuffer::dumpFB2File(char *filename)
{
   dumpPixels2File(0, 0, width-1, height-1, filename);
   return;
}


/* write the viewport to the specified file. */
void FrameBuffer::dumpVP2File(char *filename)
{
   dumpPixels2File(vp_ul_x, vp_ul_y, vp_lr_x, vp_lr_y, filename);
   return;
}


/* write a rectangular sub array of pixels to the specified file */
/*
http://stackoverflow.com/questions/2693631/read-ppm-file-and-store-it-in-an-array-coded-with-c
*/
void FrameBuffer::dumpPixels2File(int ul_x, int ul_y, int lr_x, int lr_y, char *filename)
{
   int p_width  = lr_x - ul_x + 1;
   int p_height = lr_y - ul_y + 1;

   /* need to create the file */
   FILE *fp = fopen(filename, "wb");
   if (!fp)
   {
      fprintf(stderr, "ERROR! Could not create file %s\n", filename);
      fflush(stderr);
      exit(-1);
   }
   //fprintf(stderr, "Created file %s\n", filename);

   /* create the PPM header information first */
   char buf[50];
   sprintf(buf, "P6%c%d %d%c%d%c", 13, p_width, p_height, 13, 255, 13);

   /* write the PPM header information */
   if (fwrite(buf, strlen(buf), 1, fp) != 1)
   {
      fprintf(stderr, "ERROR! Could not write to file %s\n", filename);
      fflush(stderr);
      exit(-1);
   }

   /* write the pixel data to the file */
   for (int n = 0; n < p_height; n++)
   {  // write one row of pixels at a time

      // read from the top row of the data buffer
      // down towards the bottom row
      if (fwrite(pixel_buffer+((ul_y+n)*width + ul_x)*3, p_width*3, 1, fp) != 1)
      {
         fprintf(stderr, "ERROR! Could not write to file %s\n", filename);
         fflush(stderr);
         exit(-1);
      }

      // read them from the bottom row of the data buffer
      // up towards the top row
//      if (fwrite(pixel_buffer+((lr_y-n)*width + ul_x)*3, p_width*3, 1, fp) != 1)
//      {
//         fprintf(stderr, "ERROR! Could not write to file %s\n", filename);
//         fflush(stderr);
//         exit(-1);
//      }
   }

   fclose(fp);
   return;
}


/* A simple test of the framebuffer. */
void FrameBuffer::fbTest()
{
   for (int y = 0; y < height; y++)
   {
      for (int x = 0; x < width; x++)
      {
         unsigned char gray = (unsigned char)( (x|y)%255 );
         *(pixel_buffer + (y*width + x)*3 + 0) = gray;
         *(pixel_buffer + (y*width + x)*3 + 1) = gray;
         *(pixel_buffer + (y*width + x)*3 + 2) = gray;
      }
   }
   return;
}
