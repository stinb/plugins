#include <iostream>

class Array {
private:
  int *ptr;
  int size;
public:
  Array& operator= (const Array &rhs);
};

// UndCC_Valid
Array& Array::operator= (const Array &rhs)
{
  /* SELF ASSIGNMENT CHECK */
  if(this != &rhs)
  {
    delete [] ptr;
    ptr = new int [rhs.size];
    size = rhs.size;
    for(int i = 0; i < size; i++)
      ptr[i] = rhs.ptr[i];    
  }  
  return *this; 
}

class Base {
};
class Derived : public Base {
public:
  Derived& operator= (const Derived& d); // UndCC_Violation
  Derived& operator= (Derived&& d);
};


Derived& Derived::operator= (const Derived& d)
{
  Base::operator = (d);
  return *this;
}
Derived& Derived::operator= (Derived&& d)
{
  Base::operator= (std::move(d));
  return *this;
}
