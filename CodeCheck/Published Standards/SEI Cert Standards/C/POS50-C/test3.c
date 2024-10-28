// SEI CERT C

#if __has_include(<pthread.h>)

#include <pthread.h>

/* Declare val as a global static variable */
int val;

void *childThread(void *val) {
  /* Correctly prints 1 */
  int *res = (int *)val;
  printf("Result: %d\n", *res);
  return NULL;
}

void createThread(pthread_t *tid) {
  val = 1;
  int result;
  if ((result = pthread_create(tid, NULL, childThread, &val)) != 0) { // UndCC_Valid
    /* Handle error */
  }
}

int main(void) {
  pthread_t tid;
  createThread(&tid);
  int result;
  if ((result = pthread_join(tid, NULL)) != 0) {
    /* Handle error */
  }
  return 0;
}

#endif // __has_include(<pthread.h>)
