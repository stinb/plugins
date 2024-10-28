#include <signal.h>
#include <stddef.h>

#ifdef SIGUSR1

void handler(int signum) {
  /* Handle signal */
}

void func(void) {
  struct sigaction act;  // UndCC_Valid - sigaction() function assigns handlers
                         // to signals in a similar manner to signal() but allows the caller to explicitly set persistence
  act.sa_handler = handler;
  act.sa_flags = 0;
  if (sigemptyset(&act.sa_mask) != 0) {
    /* Handle error */
  }
  if (sigaction(SIGUSR1, &act, NULL) != 0) {
    /* Handle error */
  }
}

#endif // SIGUSR1
