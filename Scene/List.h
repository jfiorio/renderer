#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include "StaticBlock.h"

/** 
   Design Rationale: we need a fast list type that avoids as many run-time checks as possible.
   We actually need several such types of list, but each should support these operations: 
   1) initialize its node's forward links
   2) divide itself into n sub-lists
   3) (re)form itself from a merging of n sub-lists
   We might have a base list class that implements these operations and derived classes for
   each list subtype. Unfortunately the last 2 operations would require either an explicit downcast
   or individual stubs to be present in each derived class to return the appropriate type.
  
   Subtypes of lists include:
   - lists with statically allocated internal storage for items
   - lists with dynamically allocated internal storage for items
   - lists without internal storage for items
   - lists that are used strictly for allocating items to other lists
   - lists that are strictly for being manipulated
   - lists that provide no intrinsic operations
   
   We use policies to implement each requirement and satisfy the many permutations of list type 
   that are possible. This also solves the explicit downcast problem since the order of 
   inheritance is reversed. (see policy.txt)
  
   The operations implemented in the list operation policies have specific usage requirements.
   Extraneous code is avoided at all costs for these operations, so they must be used appropriately.
   Failing to use an operation in the appropriate case may involve the skipping of some necessary 
   check and will thus result in undefined behavior. Functions are inlined to avoid unneccesary
   overhead which becomes a performance bottleneck when performing repetitive operations on large 
   lists.
**/

// For differentiating class types from primitive types

#ifndef _GLIBCXX_TYPE_TRAITS
template<typename T>
class is_class 
{
  typedef char yes[1];
  typedef char no [2];
  template<typename C> static yes& test(int C::*); // selected if C is a class type
  template<typename C> static no&  test(...);      // selected otherwise
  public:
  static bool const value = sizeof(test<T>(0)) == sizeof(yes);
};
#endif

// A generic list node concatenates the generic type's fields and a forward link
// - If the generic type is primitive, a single 'value' field is used instead
template <typename T, bool B = is_class<T>::value>
struct ListNode : public T
{
  ListNode<T> *next;
  inline operator T() { return this; }
};

template <typename T>
struct ListNode<T, false>
{
  T value;
  ListNode<T> *next;
  inline operator T() { return this; }
};

// Enums for policy specializations
enum StoragePolicyType
{
  no_storage,
  _static,
  dynamic
};

enum OperationPolicyType
{
  no_operation,
  manipulate,
  allocate
};

// StoragePolicyType: 0 = none
// List storage not internal to the host class. 
template <StoragePolicyType type, typename T, int size = 0>
class ListStoragePolicy
{
  public:
  ListNode<T> *items;
  ListStoragePolicy(int dsize) : items(0) {};
  ~ListStoragePolicy() {};
  bool inited = false;
};

// StoragePolicyType: 1 = static
// A statically allocated block of nodes of specified size internal
// to the host class comprises the list.
template <typename T, int size>
class ListStoragePolicy<_static, T, size>
{
  private:
  StaticBlock<ListNode<T>, size> mem;
  
  public:
  ListNode<T> *items; 
  ListStoragePolicy(int dsize)
  {
    items = size ? (ListNode<T>*)&mem.data[0] : 0;
  }
  ~ListStoragePolicy() {};
};

// StoragePolicyType: 2 = dynamic
// A dynamically allocated block of nodes of specified size internal
// to the host class comprises the list.
template <typename T, int size>
class ListStoragePolicy<dynamic, T, size>
{
  public:
  ListNode<T> *items;
  ListStoragePolicy(int dsize)
  {
    int msize = size ? size : dsize;
    items = msize ? new ListNode<T>[msize] : 0;
  }
  ~ListStoragePolicy()
  {
    if (items)
      delete items;
  }
};

// Generic structural base for all lists
template <typename T>
struct ListInfo
{
  ListNode<T> *head;
  ListNode<T> *tail;
  int size;
};

// OperationPolicyType: 0 = none
// The list does not have intrinsic operations
template <OperationPolicyType type, typename T>
class ListOperationPolicy : public ListInfo<T> {};

// OperationPolicyType: 1 = manipulate
// The list provides operations to manipulate itself
// -Note: if the list uses an internal storage policy, these operations
//  can introduce foreign segments of allocated node 
template <typename T>
class ListOperationPolicy<manipulate, T> : public ListInfo<T>
{
  public:  
  ListNode<T> sentinel;
  inline void add(ListNode<T> *item)
  {
    if (!this->size) {
      this->head = this->tail = item;
      this->size = 1;
      item->next = 0;
      sentinel.next = this->head;
    }
    else {
      this->tail->next = item;
      this->tail = item;
      item->next = 0;
      this->size++;
    }
  }
  inline void add(T *item) { add((ListNode<T>*)item); }
  
  inline void insert(ListNode<T> *prev, ListNode<T> *item)
  {
    prev->next = item;
    if (prev == this->tail) 
      this->tail = item;
    this->size++;
  }

  inline void remove(ListNode<T> *prev, ListNode<T> *item)
  {
    if (item == this->head)
    {
      this->head = this->head->next;
      sentinel.next = this->head;
    }
    else
    {
      prev->next = item->next;
      if (item == this->tail)
        this->tail = prev;
    }
    this->size--;
  }
  
  inline void reset()
  {
    this->head = 0;
    this->tail = 0;
    this->size = 0;
    sentinel.next = 0;
  }
  
  /*
  inline void remove(ListNode<T> *prev, ListNode<T> *item)
  {
    prev->next = item->next;
    if (item == this->tail) 
      this->tail = prev;
    this->size--;
  }
  
  inline void removeHead()
  {
    this->head = this->head->next;
    this->size--;
  }
  */
};

// OperationPolicyType: 2 = allocate
// The list provides operations to allocate segments of nodes
// to other lists
template <typename T>
class ListOperationPolicy<allocate, T> : public ListInfo<T>
{
  public:
  // allocate a single node from the list
  inline ListNode<T> *allocateNode()   
  {
    ListNode<T> *node = this->head;
    this->head = this->head->next;
    node->next = 0;
    this->size--;
    return node;
  }
  
  // allocate count nodes from the list; 
  // return first and last (byref) nodes in the group allocated
  inline ListNode<T> *allocateNodes(int count, ListNode<T> *&last)
  {
    last = this->head;
    for (int i=0; i<count-1; i++)
      last = last->next;
    return endAllocateNodes(count, last);
  }
  
  // reclaim a single node 
  inline void reclaimNode(ListNode<T> *node)
  {
    node->next = this->head;
    this->head = node;
    this->size++;
  }
  
  // reclaim a block of nodes
  inline void reclaimNodes(int count, ListNode<T> *first, ListNode<T> *last)
  {
    last->next = this->head;
    this->head = first;
    this->size += count;
  }
  
  // return head of the list; 
  // use to iterate and occupy a block of nodes, keeping a counter; 
  // the last node and count must be passed to endAllocate to release the block
  inline ListNode<T> *beginAllocate()
  {
    return this->head;
  }
  
  // release count nodes from the list;
  // the last node in the block must be passed;
  // the first node in the block must be the head of the list
  inline ListNode<T> *endAllocateNodes(int count, ListNode<T> *last)
  {
    ListNode<T> *first = this->head;
    ListNode<T> *next = last->next;
    last->next = 0;  
    this->head = next;
    this->size -= count;
    return first;
  }

};

#include <stdio.h>

// Host class. Note the many permutations of 'list' types available due 
// in part to generic policy inheritance. The policy technique exploits
// the true power of multiple inheritance. 
template <typename T, 
          StoragePolicyType S = no_storage, 
          OperationPolicyType O = manipulate,
          int initSize = 0>
class LinkedList : public ListStoragePolicy<S, T, initSize>,
                   public ListOperationPolicy<O, T>
{
  public:
  LinkedList(int size = 0) : ListStoragePolicy<S, T, initSize> (size), 
                             ListOperationPolicy<O, T>()
  {
    this->head = this->items;
    this->size = size ? size : initSize;
    ListNode<T> *curItem = this->head;
    ListNode<T> *nextItem = this->head+1;
    for (int i=1; i<this->size; i++)
      (curItem++)->next = (nextItem++);
    if (curItem)
      curItem->next = 0;
    this->tail = curItem;
  }
    
  static void divide(LinkedList<T, S, O, initSize> *src, LinkedList<T, S, O, 0> **dst, int count)
  {
    ListNode<T> *ptr = src->head;
    ListNode<T> *tmp;
    int size = src->size / count;
    if (size == 0)
    {
      for (int i = 0; i < count; i++)
      {
        dst[i]->head = 0;
        dst[i]->size = 0;
        dst[i]->tail = 0;
      }
      return;
    }
    int i; for (i = 0; i < count-1; i++)
    {
      dst[i]->head = ptr;
      dst[i]->size = size;
      for (int j = 0; j < size-1; j++)
        ptr = ptr->next;
      dst[i]->tail = ptr;
      tmp = ptr->next;
      ptr->next = 0;
      ptr = tmp;
    }
    size += (src->size % count);
    dst[i]->head = ptr;
    dst[i]->tail = src->tail;
    dst[i]->size = size;
  }
  
  static void merge(LinkedList<T, S, O, 0> **src, LinkedList<T, S, O, initSize> *dst, int count)
  {
    int i; for (i = 0; i < count && !src[i]->size; i++);
    if (i >= count) 
    { 
      dst->head = 0; 
      dst->tail = 0; 
      dst->size = 0;
      return;
    }
    dst->head = src[i]->head;
    dst->tail = src[i]->tail;
    dst->size = src[i]->size;
    for (i++; i < count; i++)
    {
      if (src[i]->size)
      {
        dst->tail->next = src[i]->head;
        dst->tail = src[i]->tail;
        dst->size += src[i]->size;
      }
    }
  }
  
  void print(int scale)
  {
    /*
    ListNode<T> *ptr = this->head;
    ListNode<T> *prev = 0;
    while (ptr)
    {
      printf("%x -> ", (unsigned int)(unsigned long)ptr);
      if (scale > 1) 
        printf("... ->");
      for (int i=0; i<scale && ptr; i++)
      {
        prev = ptr;
        ptr = ptr->next;
      }
      if (!ptr)
        printf("%x |", (unsigned int)(unsigned long)ptr);
    }
    */
  }
      
};

/** 
   A generic pool satisfies the following
   - it is a list of items of type T
   - either its items are stored internally
     OR they are stored internally to some other pool that stores items of type T
   - internally stored items are allocated statically
   - it is used to allocate portions of its internally or externally stored items
     in the form of a list (for some use), and to reclaim those portions when they
     are done being used
   - if storage is internal, a template argument is required for the size of the 
     storage buffer; since this argument must be embedded in the type name, then 
     at compile time, the compiler will automatically know to instantiate a special 
     structure that includes a static array of that size and the appropriate type
     for every declaration of a Pool object of type which includes template arguments 
     specifying that type and that size
     NOTE: the storage buffer will not be unique under the following conditions:
     - we try to dynamically allocate and reallocate the pool object
     - we declare an array of pool objects (use divide** instead)
     - a class member is of the pool type, or a member function declares a variable of
       the pool type, and multiple instances of that class utilize the member or function
       as if the variable claims independent storage [for pool items] between instances
       (again, we can use divide to distribute a pool of PREDEFINED size amongst n 
        instances, where n is a value that can only be determined at run-time)
     EDIT: for now, pool storage is dynamically allocated at initialization. Depending on 
     compiler used, a template class's static member arrays may be placed in the .data 
     section; in this case, creating large pools with static storage will lead to bloated 
     executable size. Change all instances below of "dynamic" to "static" if this is not
     the case for your compiler.
**/
template <typename T, int initSize = 0>
class Pool : public LinkedList <T, dynamic, allocate, initSize>
{
  private:
  inline void split(ListNode<T> *first, ListNode<T> *last, int size, LinkedList<T> *dst)
  {
    dst->head = first;
    dst->tail = last;
    dst->size = size;
  }
  
  public:
  Pool() {}
  Pool(int size) : LinkedList<T, dynamic, OperationPolicyType::allocate, initSize>(size) {}
  inline ListNode<T> *allocate(int size, LinkedList<T> *dst)
  {
    ListNode<T> *last;
    ListNode<T> *first = Pool<T, initSize>::allocateNodes(size, last);
    split(first, last, size, dst);
    return first;
  }
  inline void endAllocate(ListNode<T> *last, int size, LinkedList<T> *dst)
  {
    ListNode<T> *first = Pool<T, initSize>::endAllocateNodes(size, last);
    split(first, last, size, dst);
  }
  inline void reclaim(LinkedList<T> *src)
  {
    Pool<T, initSize>::reclaimNodes(src->size, src->head, src->tail);
  }
  
};
  
template <typename T>
using List = LinkedList<T>;

template <typename T, int size>
using ListPool = Pool<List<T>, size>;

#endif