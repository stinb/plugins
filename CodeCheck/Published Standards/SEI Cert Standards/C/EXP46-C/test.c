#if __has_include(<unistd.h>)

#include <unistd.h>
int main() {

  if (!(getuid() & geteuid() == 0)) { // UNDCC_Violation(lin,mac)
    /* ... */
  }

  // UndCC_Valid
  if (!(getuid() && geteuid() == 0)) {
    /* ... */
  }
}

#endif // __has_include(<unistd.h>)
