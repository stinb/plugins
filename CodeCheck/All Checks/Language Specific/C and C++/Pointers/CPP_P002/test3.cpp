// Test: Pointer must be initialized to 0, or some valid
// value when created.

class Class3 {
  static int *okay3A;
  static int *violation3A;
};
int Class3::okay3A = 0;
int Class3::violation3A = NULL;
