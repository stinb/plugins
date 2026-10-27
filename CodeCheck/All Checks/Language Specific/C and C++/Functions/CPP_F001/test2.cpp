#include "test2.h"

template <class T> Box<T>::Box() {}                // UndCC_Valid
template <class T> void Box<T>::used_mem() {}      // UndCC_Valid
template <class T> void Box<T>::unused_mem() {}    // UndCC_Violation
template <class T> int Box<T>::s = 0;              // UndCC_FalseNeg

void useBox()
{
  Box<int> b;
  b.used_mem();
}
