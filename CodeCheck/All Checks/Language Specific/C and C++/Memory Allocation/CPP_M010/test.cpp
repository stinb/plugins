#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
  float *ptr, *new_ptr;
  ptr = (float*) malloc(5*sizeof(float)); // UndCC_Violation
  if(ptr==NULL)
  {
    cout << "Memory Allocation Failed";
    exit(1);
  }

  /* Initializing memory block */
  for (int i=0; i<5; i++)
  {
    ptr[i] = i*1.5;
  }

  /* reallocating memory */
  new_ptr = (float*) realloc(ptr, 10*sizeof(float)); // UndCC_Violation
  if(new_ptr==NULL)
  {
    cout << "Memory Re-allocation Failed";
    exit(1);
  }
  
  /* Initializing re-allocated memory block */
  for (int i=5; i<10; i++)
  {
    new_ptr[i] = i*2.5;
  }
  cout << "Printing Values" << endl;
  
  for (int i=0; i<10; i++)
  {
    cout << new_ptr[i] << endl;
  }
  free(new_ptr); // UndCC_Violation
  
  return 0;
}
