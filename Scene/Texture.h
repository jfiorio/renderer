#ifndef TEXTURE_H
#define TEXTURE_H

typedef unsigned int GLuint; /* needed for OpenGL renderer */


class Texture
{
   private:
   public:
      int width;
      int height;
      unsigned char * data; /* contains the pixel color data for the texture */

      GLuint tex;           /* handle to opengl texture (only used by opengl) */

      Texture(char * name); /* name of the file to load texture data from */

      Texture(int _width, int _height, unsigned char * _data); /* create a Texture object
                                                                 directly from texture data */
      ~Texture();

      void getFloatColor(int u, int v, float * color);
};

#endif  /* TEXTURE_H */
