// Multi-pointer declaration

static char hi[3] = "hi";
static char **hi_ptr = (char **) &hi; // UndCC_Violation(1,2)

extern void multi_ptr_parameters(
  const volatile int param,
  const volatile int * const volatile restrict param_ptr,
  const volatile int * const volatile restrict * const volatile restrict param_ptr_ptr // UndCC_Violation(1,2)
);

static void multi_ptr_objects(void)
{
  const volatile int object;
  const volatile int * const volatile restrict object_ptr;
  const volatile int * const volatile restrict * const volatile restrict object_ptr_ptr; // UndCC_Violation(1,2)
}

extern void multi_array_parameters(
  const volatile int param,
  const volatile int param_ptr[4],
  const volatile int param_ptr_ptr[4][4], // UndCC_Violation(2)
  const volatile int *object_arr_ptr[4]   // UndCC_Violation(2) 
);

static void multi_array_objects(void)
{
  const volatile int object;
  const volatile int object_arr[4];
  const volatile int object_arr_arr[4][4];               // UndCC_Violation(2) 

  const volatile int *object_arr_ptr[4];                 // UndCC_Violation(2) 

  const volatile int initialized_object_arr[] = {
    0, 0, 0, 0
  };
  const volatile int initialized_object_arr_arr[][4] = { // UndCC_Violation(2) 
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  };
}

static char *get_hi(void)
{
  return (char *) hi;
}

static char **get_hi_ptr(void)      // UndCC_Violation(1,2)
{
  return hi_ptr;
}

static char ***get_hi_ptr_ptr(void) // UndCC_Violation(1,2)
{
  return &hi_ptr;
}
