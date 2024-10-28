// SEI CERT C

#if __has_include(<pthread.h>)

#include <pthread.h>

void *childThread(void *val) {
  /* Correctly prints 1 */
  int *res = (int *)val;
  printf("Result: %d\n", *res);
  return NULL;
}

void createThread(pthread_t *tid, int *val) {
  int result = pthread_create(tid, NULL, childThread, val); // UndCC_Violation(mac)
  if (result != 0) {
    /* Handle error */
  }
}

int main(void) {
  /* Declare val in the same function as pthread_join */
  int val = 1;
  int result;
  pthread_t tid;
  createThread(&tid, &val);
  if ((result = pthread_join(tid, NULL)) != 0) {
    /* Handle error */
  }
  return 0;
}

#endif // __has_include(<pthread.h>)
