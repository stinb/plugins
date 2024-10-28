#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

sig_atomic_t e_flag = 0; // e_flag declared without volatile keyword

void handler(int signum) {
  e_flag = 1;  // UndCC_Violation
}

int main(void) {

  if (signal(SIGINT, handler) == SIG_ERR) {
    return EXIT_FAILURE;
  }

  /* Main code loop */

  if (e_flag) {
    puts("SIGINT received.");
  }
  else {
    puts("SIGINT not received.");
  }
  return EXIT_SUCCESS;
}
