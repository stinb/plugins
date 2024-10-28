#include<stdlib.h>

class student
{
  int age;
public:
  
  // UndCC_Valid
  void * operator new(size_t size)
  {
    void * p = ::operator new(size);
    
    return p;
  }
};
 
//Non-compliant
void * operator new(size_t size) // UndCC_Violation
{
    void * p = malloc(size);
    return p;
}
 

 
