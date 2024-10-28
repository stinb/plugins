// Custom

#include <stddef.h>
#include <stdint.h>

typedef uint32_t sa_family_t;
typedef uint32_t socklen_t;

struct sockaddr {
  sa_family_t sa_family;
  char        sa_data[];
};

size_t len = sizeof(uint32_t);
int flags = 0;

int error_limit = sizeof(uint32_t);

void fn_bad_recvfrom(int sock, struct sockaddr *src_addr, socklen_t *src_len) {
  /* sock is a connected TCP socket */

  uint32_t num;

  if (recvfrom(sock, (void *) &num, len, flags, src_addr, src_len) < error_limit) { // UndCC_Violation
    /* Handle error */
  }

  printf("We received %u from the network!\n", (unsigned int) num);
}

void fn_good_recvfrom(int sock, struct sockaddr *src_addr, socklen_t *src_len) {
  /* sock is a connected TCP socket */

  uint32_t num;

  if (recvfrom(sock, (void *) &num, len, flags, src_addr, src_len) < error_limit) {
    /* Handle error */
  }

  num = ntohl(num);
  printf("We recieved %u from the network!\n", (unsigned int)num);
}

void fn_bad_send(int sock) {
  /* sock is a connected TCP socket */

  uint32_t num;

  if (send(sock, (void *) &num, len, flags) < error_limit) { // UndCC_Violation
    /* Handle error */
  }

  num = ntohl(num); // Wrong function
  printf("We received %u from the network!\n", (unsigned int) num);
}

void fn_good_send(int sock) {
  /* sock is a connected TCP socket */

  uint32_t num;

  if (send(sock, (void *) &num, len, flags) < error_limit) {
    /* Handle error */
  }

  num = htonl(num);
  printf("We recieved %u from the network!\n", (unsigned int)num);
}

void fn_bad_sendto(int sock, struct sockaddr *dest_addr, socklen_t *dest_len) {
  /* sock is a connected TCP socket */

  uint32_t num;

  if (sendto(sock, (void *) &num, len, flags, dest_addr, dest_len) < error_limit) { // UndCC_Violation
    /* Handle error */
  }

  num = htonl(0); // Wrong argument
  printf("We received %u from the network!\n", (unsigned int) num);
}

void fn_good_sendto(int sock, struct sockaddr *dest_addr, socklen_t *dest_len) {
  /* sock is a connected TCP socket */

  uint32_t num;

  if (sendto(sock, (void *) &num, len, flags, dest_addr, dest_len) < error_limit) {
    /* Handle error */
  }

  num = htonl(num);
  printf("We recieved %u from the network!\n", (unsigned int)num);
}
