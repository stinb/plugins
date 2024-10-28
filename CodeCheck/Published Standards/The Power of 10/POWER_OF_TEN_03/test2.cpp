static void dynamic_cpp()
{
  int* ptr = new int;         // UndCC_Violation
  delete ptr;                 // UndCC_Violation

  int* arr_ptr = new int[10]; // UndCC_Violation
  delete[] arr_ptr;           // UndCC_Violation
}
