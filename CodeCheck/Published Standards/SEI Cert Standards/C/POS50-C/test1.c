// SEI CERT C

#if __has_include(<pthread.h>)

#include <pthread.h>

__thread int val;

void *childThread(void *val) {
  int *res = (int *)val;

  printf("Result: %d\n", *res);
  return NULL;
}

void *createThread(void *childTid) {
  pthread_t *tid = (pthread_t *)childTid;
  int result;
  val = 1;
  if ((result = pthread_create(tid, NULL, childThread, &val)) != 0) { // UndCC_Violation(lin,mac)
    /* Handle error */
  }
  return NULL;
}

void *empty(void *arg) {
  /* Function that does nothing */
  val = 0;
  return NULL;
}

int main(void) {
  pthread_t parentTid, childTid, emptyTid;
  int result;

  /*
   * createThread() may complete before childThread(), and
   * the thread-local variable, val, belonging to createThread(),
   * may no longer be valid when childThread() runs.
   */
  if ((result = pthread_create(&parentTid, NULL, createThread, &childTid)) != 0) { // UndCC_Violation(mac)
    /* Handle error */
  }
  if ((result = pthread_join(parentTid, NULL)) != 0) {
    /* Handle error */
  }
  if ((result = pthread_create(&emptyTid, NULL, empty, NULL)) != 0) { // UndCC_Valid
    /* Handle error */
  }
  if ((result = pthread_join(emptyTid, NULL)) != 0) {
    /* Handle error */
  }
  if ((result = pthread_join(childTid, NULL)) != 0) {
    /* Handle error */
  }
  return 0;
}

#endif // __has_include(<pthread.h>)
