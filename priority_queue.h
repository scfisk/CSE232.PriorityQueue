/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>
#include "vector.h"

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

/*************************************************
 * P QUEUE
 * Create a priority queue.
 *************************************************/
template<class T>
class priority_queue
{
   friend class ::TestPQueue; // give the unit test class access to the privates
   template <class TT>
   friend void swap(priority_queue<TT>& lhs, priority_queue<TT>& rhs);
public:

   //
   // construct
   //
   priority_queue()
   {
   }

   priority_queue(const priority_queue &  rhs)
   {
       // copy container from rhs
       container = rhs.container;
   }
   priority_queue(priority_queue && rhs)  
   {
       // use move on the rhs container
       container = std::move(rhs.container);
   }
   template <class Iterator>
   priority_queue(Iterator first, Iterator last) 
   {
       container.reserve(last - first);
       for (Iterator it = first; it != last; it++)
           push(*it);
   }
   explicit priority_queue (custom::vector<T> && rhs)
   {
      container = std::move(rhs);
      heapify();
   }
   explicit priority_queue (custom::vector<T>& rhs)
   {
      container = rhs;
      heapify();
   }
  ~priority_queue() {}

   //
   // Access
   //
   const T & top() const;

   //
   // Insert
   //
   void  push(const T& t);
   void  push(T&& t);     

   //
   // Remove
   //
   void  pop(); 

   //
   // Status
   //
   size_t size()  const 
   { 
      return container.size();
   }
   bool empty() const 
   {
       return size() == size_t(0);
   }
   
private:

   void heapify();                            // convert the container in to a heap
   bool percolateDown(size_t indexHeap);      // fix heap from index down. This is a heap index!

   custom::vector<T> container; 

};

/************************************************
 * P QUEUE :: TOP
 * Get the maximum item from the heap: the top item.
 ***********************************************/
template <class T>
const T & priority_queue<T>::top() const
{
    if (container.empty())
        throw std::out_of_range("std:out_of_range");

    return container[0];
}

/**********************************************
 * P QUEUE :: POP
 * Delete the top item from the heap.
 **********************************************/
template <class T>
void priority_queue<T>::pop()
{
   if (container.empty())
      return;

   std::swap(container[0], container[container.size() - 1]);
   container.pop_back();

   if (!container.empty())
      percolateDown(1);   // heap index of the root is 1
}


/*****************************************
 * P QUEUE :: PUSH
 * Add a new element to the heap, reallocating as necessary
 ****************************************/
template <class T>
void priority_queue <T> :: push(const T & t)
{
   // 1. Put new element at the end
   container.push_back(t);

   // 2. Percolate up
   if (container.size() <= 1)
      return;

   size_t index = container.size() - 1;
   while (index > 0)
   {
      size_t parent = (index - 1) / 2;

      if (container[parent] < container[index])
      {
         std::swap(container[parent], container[index]);
         index = parent;
      }
      else
         break;
   }
}

template <class T>
void priority_queue <T> :: push(T && t)
{
   // 1. Move new element to the end
   container.push_back(std::move(t));

   // 2. Percolate up
   if (container.size() <= 1)
      return;

   size_t index = container.size() - 1;
   while (index > 0)
   {
      size_t parent = (index - 1) / 2;

      if (container[parent] < container[index])
      {
         std::swap(container[parent], container[index]);
         index = parent;
      }
      else
         break;
   }
}

/************************************************
 * P QUEUE :: PERCOLATE DOWN
 * The item at the passed index may be out of heap
 * order. Take care of that little detail!
 * Return TRUE if anything changed.
 ************************************************/
template <class T>
bool priority_queue <T> :: percolateDown(size_t indexHeap)
{
   bool changed = false;
   size_t n = container.size();

   // indexHeap is 1-based; 0 or > n means "nothing to do"
   if (indexHeap == 0 || indexHeap > n)
      return false;

   while (true)
   {
      size_t iLeft  = indexHeap * 2;       // 1-based
      size_t iRight = indexHeap * 2 + 1;   // 1-based

      // no children
      if (iLeft > n)
         break;

      // pick bigger child (still 1-based)
      size_t iBigger = iLeft;
      if (iRight <= n && container[iLeft - 1] < container[iRight - 1])
         iBigger = iRight;

      // if child bigger than parent, swap
      if (container[indexHeap - 1] < container[iBigger - 1])
      {
         std::swap(container[indexHeap - 1], container[iBigger - 1]);
         indexHeap = iBigger;
         changed = true;
      }
      else
      {
         break;
      }
   }

   return changed;
}


/************************************************
 * P QUEUE :: HEAPIFY
 * Turn the container into a heap.
 ************************************************/
template <class T>
void priority_queue<T>::heapify()
{
   size_t n = container.size();
   if (n <= 1)
      return;

   // i is a *heap index* (1-based), so internal nodes are floor(n/2) .. 1
   for (size_t i = n / 2; i >= 1; --i)
   {
      percolateDown(i);
      if (i == 1) break;   // avoid size_t underflow
   }
}


/************************************************
 * SWAP
 * Swap the contents of two priority queues
 ************************************************/
template <class T>
inline void swap(custom::priority_queue<T>& lhs,
                 custom::priority_queue<T>& rhs)
{
    lhs.container.swap(rhs.container);
}


};

