void myfunc( int * param1, // UndCC_Valid - Addresses an object which is modified
const int * param2, // UndCC_Valid - Addresses an object which is not modified
      int * param3, // UndCC_Violation - Addresses an object which is not modified
      int * const param4) // UndCC_Violation - Addresses an object which is not modified


{
  *param1 = *param2 + *param3 + *param4;
// Data at address param3 and param4 have not been changed
}

