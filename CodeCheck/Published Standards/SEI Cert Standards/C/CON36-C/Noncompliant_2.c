#if __has_include(<threads.h>)

#include <stdio.h>
#include <threads.h>

mtx_t mutex;
cnd_t condition;

int shared_data = 0;

int wait_and_modify(void *arg) {
  mtx_lock(&mutex);

  // Failure to enclose cnd_wait in a loop
  cnd_wait(&condition, &mutex);  // UndCC_Violation(win,lin)

  // After waking up, modify the shared data
  shared_data = 99;

  mtx_unlock(&mutex);

  return 0;
}

int main(void) {
  thrd_t thread;

  cnd_init(&condition);
  mtx_init(&mutex, mtx_plain);

  thrd_create(&thread, wait_and_modify, NULL);

  // Simulate some work
  sleep(2);

  // Modify the shared data
  mtx_lock(&mutex);
  shared_data = 42;
  mtx_unlock(&mutex);

  // Signal the condition
  cnd_signal(&condition);

  thrd_join(thread, NULL);

  cnd_destroy(&condition);
  mtx_destroy(&mutex);

  return 0;
}

#endif // __has_include(<threads.h>)
