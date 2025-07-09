#include <pthread.h>
void f(pthread_mutex_t *mutex) {
  pthread_mutex_lock(mutex); // UndCC_Violation

  /* access shared resource */


  pthread_mutex_unlock(mutex);
}
