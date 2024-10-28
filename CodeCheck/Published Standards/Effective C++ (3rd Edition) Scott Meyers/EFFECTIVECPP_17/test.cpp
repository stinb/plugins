#include <iostream>
#include <memory>
int main () {
  int *ptr1 = new int; /* UNDCC_Violation */
  float *ptr2 = new float(223.324); /* UNDCC_Violation */
  std::shared_ptr<int> p1 (new int); /* UNDCC_Valid */
  
  delete ptr1;
  delete ptr2;
  return 0;
}
