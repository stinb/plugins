#include <pthread.h>
int f(pthread_mutex_t *mutex) {
  int result;

  result = pthread_mutex_lock(mutex);
  if (0 != result)
  return result;


  /* access shared resource */


  return pthread_mutex_unlock(mutex);
}
