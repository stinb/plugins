// SEI CERT C

#if __has_include(<pthread.h>)

#include <pthread.h>

/* Declare val as a thread-local value */
__thread int val;

void *childThread(void *val) {
  /* Correctly prints 1 */
  int *res = (int *)val;
  printf("Result: %d\n", *res);
  return NULL;
}

void createThread(pthread_t *tid) {
  val = 1;
  int result = pthread_create(tid, NULL, childThread, &val); // UndCC_Violation(mac)
  if (result != 0) {
    /* Handle error */
  }
}

int main(void) {
  pthread_t tid;
  int result;
  createThread(&tid);
  if ((result = pthread_join(tid, NULL)) != 0) {
    /* Handle error */
  }
  return 0;
}

#endif // __has_include(<pthread.h>)
