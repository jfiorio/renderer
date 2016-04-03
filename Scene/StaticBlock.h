#ifndef STATIC_BLOCK_H
#define STATIC_BLOCK_H

#include "static_counter.h"

template <typename T>
struct static_counter<T, 0>
{
  static int constexpr next() { return 0; }
};

template <typename T, int size, int id = static_counter<T, size>::next()>
struct StaticBlock
{
  static T data[size];
};

template <typename T>
struct StaticBlock<T, 0, 0>
{
  const static T data[1]; // force assert on write a static block of size 0
};

template <typename T, int size, int id>
T StaticBlock<T, size, id>::data[size] = { T() };

template <typename T>
const T StaticBlock<T, 0, 0>::data[1] = { T() };

#endif

