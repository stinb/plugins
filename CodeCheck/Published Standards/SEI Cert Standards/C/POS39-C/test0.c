// SEI CERT C

#include <stdint.h>

void fn_bad_recv(int sock) {
  /* sock is a connected TCP socket */

  uint32_t num;

  if (recv(sock, (void *)&num, sizeof(uint32_t), 0) < (int)sizeof(uint32_t)) { // UndCC_Violation
    /* Handle error */
  }

  printf("We received %u from the network!\n", (unsigned int)num);
}

void fn_good_recv(int sock) {
  /* sock is a connected TCP socket */

  uint32_t num;

  if (recv(sock, (void *)&num, sizeof(uint32_t), 0) < (int)sizeof(uint32_t)) {
    /* Handle error */
  }

  num = ntohl(num);
  printf("We recieved %u from the network!\n", (unsigned int)num);
}
