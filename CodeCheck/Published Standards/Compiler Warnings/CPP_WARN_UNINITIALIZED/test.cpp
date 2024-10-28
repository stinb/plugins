#include <stdlib.h>

// extern
#include "test.hpp"
extern int extern_uninit;
extern int extern_init;

// _Thread_local extern
_Thread_local extern int thread_extern_uninit;
_Thread_local extern int thread_extern_init;

// _Thread_local static
_Thread_local static int thread_static_uninit;
_Thread_local static int thread_static_init = 1;

// static
static int static_uninit;
static int static_init = 1;

void fn_int(int x)
{
  // ...
}

void fn_str(char *x)
{
  // ...
}

int main()
{
  // auto
  int auto_uninit;
  int auto_init = 1;

  // allocated
  char *allocated_uninit;
  char *allocated_init = (char *) malloc(256 * sizeof(char));

  fn_int(extern_uninit);        // UndCC_Valid     - initialized (static) in other file
  fn_int(extern_init);          // UndCC_Valid     - initialized manually
  fn_int(thread_extern_uninit); // UndCC_Valid     - initialized (static) in other file
  fn_int(thread_extern_init);   // UndCC_Valid     - initialized manually
  fn_int(thread_static_uninit); // UndCC_Valid     - initialized (static) in this file
  fn_int(thread_static_init);   // UndCC_Valid     - initialized manually
  fn_int(static_uninit);        // UndCC_Valid     - initialized (static) in this file
  fn_int(static_init);          // UndCC_Valid     - initialized manually
  fn_int(auto_uninit);          // UndCC_Violation - not initialized
  fn_int(auto_init);            // UndCC_Valid     - initialized manually
  fn_str(allocated_uninit);     // UndCC_Violation - not initialized
  fn_str(allocated_init);       // UndCC_Valid     - initialized manually

  if (allocated_init != nullptr)
    free(allocated_init);

  return 0;
}
