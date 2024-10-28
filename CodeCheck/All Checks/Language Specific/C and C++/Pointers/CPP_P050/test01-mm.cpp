#include <stdint.h>
#include <cstdlib>

int ok = 0; /* UNDCC_Valid */
int* wasok = NULL; /* UNDCC_Valid */
int* notok = 0; /* UNDCC_Violation */
const int* notok01 = 0; /* UNDCC_Violation */
long* notok02 = 0; /* UNDCC_Violation */
char* notok03 = 0; /* UNDCC_Violation */
char* okay = NULL; /* UNDCC_Valid */         

int main() {
  *wasok = 0; /* UNDCC_Valid changing value */
  wasok = 0; /* UNDCC_Violation */
  ok = 0; /* UNDCC_Valid */
  notok01 = NULL; /* UNDCC_Valid */
  return 0;
}

