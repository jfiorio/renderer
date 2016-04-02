#ifndef LIGHT_H
#define LIGHT_H

class Light
{
   private:
   public:
      float ambient[4];
      float diffuse[4];
      float specular[4];
      float pos[4];     /* light's position in world coordinates */
};

#endif  /* LIGHT_H */