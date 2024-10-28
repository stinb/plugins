#include <iostream>
#include <stdexcept>
using namespace std;

// Defining function Division_A
float Division_A(float num, float den)
{
  return (num / den);    /* VIOLATION Part A- value of den could be zero */
  
}

// Defining function Division_B
float Division_B(float num, float den)
{
  if (den == 0) {
    throw runtime_error("Math error: Attempted to divide by Zero\n");
  }
  
  return (num / den);    /* Valid Part A- value of den checked before division */
  
}


int main()
{
  float numerator = 12.5;
  float denominator = 0;
  float result_A;
  float result_B;
  
  // calls Division_A function
  result_A = Division_A(numerator, denominator); // Undefined behavior
  
  cout << "The quotient of 12.5/0 is " << result_A << endl;
  
  
  // try block calls the Division_B function
  try {
    result_B = Division_B(numerator, denominator);
    
    // this will not print
    cout << "The quotient is "
         << result_B << endl;
  }
  
  // catch block catches exception thrown by Division_B function
  catch (runtime_error& e) {
    
    // prints that exception has occurred
    cout << "Exception occurred" << endl << e.what();
  }
  
  int result_C = 120 / 0;      /* UNDCC_VIOLATION Part B */
  float bad_modulo = 30 % 0;   /* UNDCC_VIOLATION Part B */
  
  return 0;
}
