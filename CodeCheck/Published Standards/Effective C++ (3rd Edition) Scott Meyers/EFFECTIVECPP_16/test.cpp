#include <string>
int main() {
  std::string *stringPtr1 = new std::string;
  std::string *stringPtr2 = new std::string[100];

  delete stringPtr1; /* UNDCC_Valid */
  delete [] stringPtr2; /* UNDCC_Valid */
  
  std::string *stringPtr3 = new std::string;
  std::string *stringPtr4 = new std::string[100];

  delete [] stringPtr3; /* UNDCC_Violation */
  delete stringPtr4; /* UNDCC_Violation */
}
