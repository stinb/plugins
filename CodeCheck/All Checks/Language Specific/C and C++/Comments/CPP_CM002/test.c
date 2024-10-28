#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void sec_delay() {
  printf("Sleep for one second (1000 ms) to exit.\n");
  usleep(1000); // UndCC_Violation
}

void two_sec_delay() {
  printf("Sleep for two seconds (2000 ms) to exit.\n");
  // Here is a comment explaining two_sec_delay
  usleep(2000); // UndCC_Valid
}


void three_sec_delay() {
  printf("Sleep for three seconds (3000 ms) to exit.\n");
  /* Here is a comment explaining three_sec_delay */
  usleep(3000); // UndCC_Valid
}

main() {

  printf("Sleeping for 1 second.\n");
  sleep(1); // UndCC_Violation

  sec_delay();
  two_sec_delay();
  three_sec_delay();

  return 0;
}
