#ifndef FRAGMENT_H
#define FRAGMENT_H

class Fragment
{
   private:
   public:
      int xIndex;        /* x coordinate in the viewport */
      int yIndex;        /* y coordinate in the viewport */
      double zCoord;     /* needed for z-buffer depth testing */
      double alpha,      /* the barycentric coordinates,              */
             beta,       /* with respect to this fragment's triangle, */
             gamma;      /* of the center of this fragment            */
      double r, g, b, a; /* color of this fragment (including transparency) */

      Fragment * nextFragment;
};

#endif  /* FRAGMENT_H */