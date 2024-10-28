#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char *argv[], const char *envp[]) {
  if (_putenv_s("MY_NEW_VAR", "new_value") != 0) {
    /* Handle error */
  }
  if (envp != NULL) {  // UndCC_Violation
    for (size_t i = 0; envp[i] != NULL; ++i) {  // UndCC_Violation
      puts(envp[i]);  // UndCC_Violation
    }
  }
  return 0;
}
