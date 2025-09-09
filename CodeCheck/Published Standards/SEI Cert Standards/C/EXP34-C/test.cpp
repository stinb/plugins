#if defined(__linux__) || defined(__APPLE__)
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

/* Declare validate_addr_form to resolve the undefined reference */
void validate_addr_form(char *addr);

void host_lookup(char *user_supplied_addr) {
  struct hostent *hp;
  in_addr_t addr;
  char hostname[64];
  
  /* routine that ensures user_supplied_addr is in the right format for conversion */
  validate_addr_form(user_supplied_addr);
  addr = inet_addr(user_supplied_addr);
  hp = gethostbyaddr(&addr, sizeof(struct in_addr), AF_INET);
  strcpy(hostname, hp->h_name); // UndCC_Violation(Lin, Mac)
}
#endif
