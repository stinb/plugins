// Custom

void f1(int n)
{
  int (**a)[n];          // UndCC_Violation - pointer to pointer to variable-length array
  typedef int (**b)[n];  // UndCC_Violation - pointer to pointer to variable-length array
  using c = int (**)[n]; // UndCC_Violation - pointer to pointer to variable-length array
}

void f2(int n)
{
  int a[n];             // UndCC_Valid - variable-length array
  typedef int b[n];     // UndCC_Valid - variable-length array
  using c = int[n];     // UndCC_Valid - variable-length array
}

void f3(int n)
{
  int *a[n];            // UndCC_Valid - variable-length array of pointers
  typedef int *b[n];    // UndCC_Valid - variable-length array of pointers
  using c = int *[n];   // UndCC_Valid - variable-length array of pointers
}
