#include <stdio.h>
#include <stdlib.h>
#include "Texture.h"


void Texture::getFloatColor(int u, int v, float * color)
{
   unsigned char *ptr = data + (((v * width) + u) * 3);
   color[0] = (float) *(ptr);
   color[1] = (float) *(ptr + 1);
   color[2] = (float) *(ptr + 2);
   return;
}


Texture::~Texture()
{
   if (data)
      free(data);
}


/* create a Texture object directly from texture data */
Texture::Texture(int _width, int _height, unsigned char * _data)
{
   width  = _width;
   height = _height;
   data   = _data;
}

/*

http://stackoverflow.com/questions/2693631/read-ppm-file-and-store-it-in-an-array-coded-with-c

*/
Texture::Texture(char * filename)
{
   char buf[30];
   int rgbSize;
   FILE *fp;
   int i;

   fp = fopen(filename, "rb");
   if (!fp)
   {
      fprintf(stderr, "ERROR! Could not read texture %s\n", filename);
      fflush(stderr);
      exit(-1);
   }
   //fprintf(stderr, "Opened texture %s\n", filename);

   /* read image format string "P6" */
   fscanf(fp, "%s", buf);

   /* read IrfanView comment */
   fscanf(fp, "%s%s%s%s", buf, buf, buf, buf);

   /* read image dimensions */
   fscanf(fp, "%d%d", &width, &height);
   fprintf(stderr, "Texture %s has dimensions width = %d, height = %d\n\n",
                    filename, width, height);

   /* read image rgb dimensions (which we don't use) */
   fscanf(fp, "%d", &rgbSize);
   //fprintf(stderr, "Texture %s has rgb dimensions rgbSize = %d\n", filename, rgbSize);

   /* make room for the data array */
   data = (unsigned char *)malloc(sizeof(unsigned char) * width * height * 4);
   if (!data)
   {
      fprintf(stderr, "ERROR! Unable to allocate memory for texture %s\n", filename);
      fflush(stderr);
      exit(-1);
   }
   //fprintf(stderr, "Allocated %d bytes for texture %s\n", 3*width*height, filename);

   unsigned char *temp = (unsigned char *)malloc(sizeof(unsigned char) * width * height * 3);
   
   /* Eat line feeds */
   while (fgetc(fp) != '\n') ;

   //read pixel data from file
   for (i = 0; i < height; i++)
   {  /* read one row of pixels at a time,
         store them from the bottom row of the data buffer
         up towards the top row */
      if (fread(temp+((height-i-1)*3*width), 3 * width, 1, fp) != 1)
      {
         fprintf(stderr, "ERROR! Could not load texture %s\n", filename);
         fflush(stderr);
         exit(-1);
      }
   }
   
   for (i = 0; i < height; i++)
   {
     for (int j = 0; j < width; j++)
     {
       data[(i*width*4)+(j*4)+0] = temp[(i*width*3)+(j*3)+0];
       data[(i*width*4)+(j*4)+1] = temp[(i*width*3)+(j*3)+1];
       data[(i*width*4)+(j*4)+2] = temp[(i*width*3)+(j*3)+2];
       data[(i*width*4)+(j*4)+3] = 0;
     }
   }
   
   free(temp);
   //fprintf(stderr, "Loaded texture %s\n", filename);


/*  this loop reads the textures upside down
   //read pixel data from file
   if (fread(data, 3 * width, height, fp) != (size_t)height)
   {
      fprintf(stderr, "ERROR! Could not load texture %s\n", filename);
      fflush(stderr);
      exit(-1);
   }
   //fprintf(stderr, "Loaded texture %s\n", filename);
*/

   fclose(fp);

   return;
}
