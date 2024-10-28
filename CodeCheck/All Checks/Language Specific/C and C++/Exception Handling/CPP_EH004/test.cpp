#include <iostream>
using namespace std;

int main () {
  
  // exception caught by value, flag violation
  try
  {
    throw 20;
  }
  catch (int e) // UndCC_Violation
  {
    cout << "An exception occurred. Exception #" << e << '\n';
  }
  
  // exception caught by reference
  try
  {
    throw 10;
  }
  catch (int &e)
  {
    cout << "An exception occurred. Exception #" << &e << '\n';
  }
  
  return 0;
}
