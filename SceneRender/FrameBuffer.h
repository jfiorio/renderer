#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class FrameBuffer
{
  private:
  public:
    int width;  /* the framebuffer's width */
    int height; /* the framebuffer's height */
    unsigned char *pixel_buffer; /* contains the pixel's color data for a rendered frame */
    float *z_buffer;            /* contains the pixel's depth data */
    
    /* coordinates of the current viewport within the framebuffer */
    int vp_ul_x; /* upper left hand corner */
    int vp_ul_y;
    int vp_lr_x; /* lower right hand corner */
    int vp_lr_y;
    
    FrameBuffer(int w, int h);
    FrameBuffer();
    ~FrameBuffer();
    
    int  getWidthFB();
    int  getHeightFB();
    void setViewport(int _vp_ul_x, int _vp_ul_y, int _vp_lr_x, int _vp_lr_y);
    int  getWidthVP();
    int  getHeightVP();
    void clearFB(unsigned char r, unsigned char g, unsigned char b); /* set a background color */
    void clearVP(unsigned char r, unsigned char g, unsigned char b);
    void clearzFB();  /* clear the z-buffer */
    void clearzVP();  /* clear the viewport's z-buffer */
    
    void   getPixelFB(int x, int y, unsigned char **c); /* get pixel from framebuffer */
    float  getDepthFB(int x, int y);          /* get pixel's depth from the framebuffer */
    void   setPixelFB(int x, int y, unsigned char *c, float z); /* set pixel in the framebuffer */
    void   getPixelVP(int x, int y, unsigned char **c); /* get pixel from within the vewport */
    float  getDepthVP(int x, int y);          /* get pixel's depth from within the vewport */

    inline void setPixelVP(int x, int y, int pixel/*unsigned char *c*/, float z)
    {
      int offset = (vp_ul_y + y)*width + vp_ul_x + x;
       unsigned char *p = pixel_buffer + offset*3;
       // p[0] = c[0];
       // p[1] = c[1];
       // p[2] = c[2];
       *(unsigned int*)(p) = (pixel) | (p[3] << 24);
       
       //tmp = _mm_set1_pi8(128);
       //_m_maskmovq(tmp, maskb, (char*)p);
    
       *(z_buffer + offset) = z;  /* ????????????????????? */
       return;
    }
    
    void dumpFB2File(char *filename); /* write the framebuffer to a file */
    void dumpVP2File(char *filename); /* write the viewport to a file */
    void dumpPixels2File(int ul_x, int ul_y, int lr_x, int lr_y, char *filename);
    
    void fbTest(); /* a simple test of the framebuffer */
};

#endif /* FRAMEBUFFER_H */
