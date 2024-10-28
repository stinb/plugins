#include <string.h>
#include <stdlib.h>
#include <stdio.h>
 
enum { BUFFERSIZE = 512 };
 
void func00(const char *input) {
  char cmdbuf[BUFFERSIZE];
  int len_wanted = snprintf(cmdbuf, BUFFERSIZE,
                            "any_cmd '%s'", input);
  if (len_wanted >= BUFFERSIZE) {
    /* Handle error */
  } else if (len_wanted < 0) {
    /* Handle error */
  } else if (system(cmdbuf) == -1) { // UndCC_Violation
    /* Handle error */
  }
}


void func01(void) {
  system("rm ~/.config"); // UndCC_Violation
  
  void* b = NULL;
  
  const char* a = NULL;

  system(a); // UndCC_Valid - ENV33-C-EX1
  popen(a, a); // UndCC_Valid - ENV33-C-EX1
  _popen(a, b); // UndCC_Valid - ENV33-C-EX1
  
  popen("echo a", b); // UndCC_Violation
  popen("echo a", b); // UndCC_Violation
  _popen("echo a", b); // UndCC_Violation
}
