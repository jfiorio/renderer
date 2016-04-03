#ifndef FRAGMENT_H
#define FRAGMENT_H

class Fragment
{
   private:
   public:
      short xIndex;        /* x coordinate in the viewport */
      short yIndex;        /* y coordinate in the viewport */
      double zCoord;     /* needed for z-buffer depth testing */
      double alpha,      /* the barycentric coordinates,              */
             beta,       /* with respect to this fragment's triangle, */
             gamma;      /* of the center of this fragment            */
      union
      {
        struct {
        char r, g, b, a; };/* color of this fragment (including transparency) */
        int pixel;
      };
};      

#endif  /* FRAGMENT_H */