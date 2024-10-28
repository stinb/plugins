// SEI CERT C

#if __has_include(<pthread.h>)

#include <pthread.h>

typedef pthread_mutex_t mutex_t;

mutex_t theLock;
int data;

int cleanupAndFinish(void) {
  int result;

  /* A user-written function that is application-dependent */
  wait_for_all_threads_to_finish();
  if ((result = pthread_mutex_destroy(&theLock)) != 0) { // UndCC_Valid
    /* Handle error */
  }
  data++;
  return data;
}

void worker(int value) {
  int result;
  if ((result = pthread_mutex_lock(&theLock)) != 0) {
    /* Handle error */
  }
  data += value;
  if ((result = pthread_mutex_unlock(&theLock)) != 0) {
    /* Handle error */
  }
}

#endif // __has_include(<pthread.h>)
