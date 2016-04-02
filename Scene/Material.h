#ifndef MATERIAL_H
#define MATERIAL_H

class Material
{
   private:
   public:
      float ambient[4];
      float diffuse[4];
      float specular[4];
      float s;          /* shininess coefficient */
};

#endif  /* MATERIAL_H */