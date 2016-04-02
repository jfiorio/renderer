#ifndef NODE_H
#define NODE_H

#include <string.h>    /* for definition of NULL */
#include "Triangle.h"

class TriangleListNode
{
   private:
   public:
      Triangle *t;
      TriangleListNode *next;

      TriangleListNode()
      {
         next = NULL;
         t = NULL;
      }
};

#endif  /* NODE_H */
