#include "Context.h"
#include <new>
Context::Context() : parent(0), children(0), childCount(0) {};

// private constructor/destructor for children
Context::Context(Context *parent)
{
  scene = parent->scene;
  fb = parent->fb;
  triangles = new List<Triangle>;
  trianglePool = new Pool<Triangle>;
  fragmentPool = new Pool<Fragment>;
  
  children = 0;
  childCount = 0;
  this->parent = parent;
}

Context::~Context()
{
  if (!parent)
  {
    delete triangles;
    delete trianglePool;
    delete fragmentPool;
  }
}

// setters
void Context::set(Scene *scene, FrameBuffer *fb)
{
  this->scene = scene;
  this->fb = fb;
  this->triangles = (List<Triangle>*)&scene->triangles;
  this->trianglePool = scene->trianglePool;
  this->fragmentPool = scene->fragmentPool;
}

bool Context::resetChildren(int count, int flag)
{
  bool check = (childCount != count) || (flag & 1);
  if (check) deleteArray(children, childCount);
  check |= !children;
  if (check) 
  {
    children = newArray(this, count);
    childCount = count;
  }
  return check;
}

// array operations
Context *Context::newArray(Context *parent, int size)
{ 
  void* mem = operator new[](size * sizeof(Context));
  Context* ptr = static_cast<Context*>(mem);
  for(int i = 0; i < size; i++) {
    new(&ptr[i]) Context(parent);
  }
  return ptr;
}

void Context::deleteArray(Context *array, int size)
{
  for(int i = size - 1; i >= 0; --i) {
    array[i].~Context();
  }
  operator delete[]((void*)array);
}

Pool<Triangle> **Context::getTrianglePoolPtrs(Context *array, int size)
{
  Pool<Triangle> **trianglePoolPtrs = new Pool<Triangle>*[size];
  for (int i=0; i<size; i++)
    trianglePoolPtrs[i] = array[i].trianglePool;
  return trianglePoolPtrs;
}
 
Pool<Fragment> **Context::getFragmentPoolPtrs(Context *array, int size)
{
  Pool<Fragment> **fragmentPoolPtrs = new Pool<Fragment>*[size];
  for (int i=0; i<size; i++)
    fragmentPoolPtrs[i] = array[i].fragmentPool;
  return fragmentPoolPtrs;
}

List<Triangle> **Context::getTriangleListPtrs(Context *array, int size)
{
  List<Triangle> **triangleListPtrs = new List<Triangle>*[size];
  for (int i=0; i<size; i++)
    triangleListPtrs[i] = array[i].triangles;
  return triangleListPtrs;
}

void Context::setTrianglePoolPtrs(Context *array, int size, Pool<Triangle> **trianglePoolPtrs)
{
  for (int i=0; i<size; i++)
    array[i].trianglePool = trianglePoolPtrs[i];
  delete[] trianglePoolPtrs;
}
 
void Context::setFragmentPoolPtrs(Context *array, int size, Pool<Fragment> **fragmentPoolPtrs)
{
  for (int i=0; i<size; i++)
    array[i].fragmentPool = fragmentPoolPtrs[i];
  delete[] fragmentPoolPtrs;
}

void Context::setTriangleListPtrs(Context *array, int size, List<Triangle> **triangleListPtrs)
{
  for (int i=0; i<size; i++)
    array[i].triangles = triangleListPtrs[i];
  delete[] triangleListPtrs;
}

#define dptrig LinkedList<Triangle, dynamic, allocate>**
#define dpfrag LinkedList<Fragment, dynamic, allocate>**

// fork/join
Context *Context::fork(Context *src, int count, int flag)
{
  if (src->resetChildren(count, flag) /* || compare pools for change */)
  {
    Pool<Fragment> **fragmentPoolPtrs = getFragmentPoolPtrs(src->children, count);
    Pool<Fragment>::divide(src->fragmentPool, (dpfrag)fragmentPoolPtrs, count);
    setFragmentPoolPtrs(src->children, count, fragmentPoolPtrs);
  }
  Context *dst = src->children;
  if (!(flag & 4))
  {
    Pool<Triangle> **trianglePoolPtrs = getTrianglePoolPtrs(src->children, count);
    Pool<Triangle>::divide(src->trianglePool, (dptrig)trianglePoolPtrs, count);
    setTrianglePoolPtrs(src->children, count, trianglePoolPtrs);
  }
  if (!(flag & 2))
  {
    List<Triangle> **triangleListPtrs = getTriangleListPtrs(src->children, count);
    List<Triangle>::divide(src->triangles, triangleListPtrs, count);
    setTriangleListPtrs(src->children, count, triangleListPtrs);  
        
  }
  return dst;
}

Context *Context::join(Context *src, int count, int flag)
{ 
  Context *dst = src->parent;
  if (flag & 1)
  {
    Pool<Fragment> **fragmentPoolPtrs = getFragmentPoolPtrs(src, count);
    Pool<Fragment>::merge((dpfrag)fragmentPoolPtrs, dst->fragmentPool, count);
    delete[] fragmentPoolPtrs;    
  }
  if (!(flag & 4))
  {
    Pool<Triangle> **trianglePoolPtrs = getTrianglePoolPtrs(src, count);
    Pool<Triangle>::merge((dptrig)trianglePoolPtrs, dst->trianglePool, count);
    delete[] trianglePoolPtrs;
  }
  if (!(flag & 2))
  {
    List<Triangle> **triangleListPtrs = getTriangleListPtrs(src, count);
    List<Triangle>::merge(triangleListPtrs, dst->triangles, count);  
    delete[] triangleListPtrs;
  }
  return dst;
}