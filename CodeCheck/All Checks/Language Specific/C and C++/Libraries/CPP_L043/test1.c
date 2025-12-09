// Custom


#include <stdint.h>
#include <string.h>
#include <threads.h> // C11 threads not supported on Mac/Linux


cnd_t condition_a;
cnd_t condition_b;
mtx_t mutex_a;
mtx_t mutex_b;
thrd_t thread_a;
thrd_t thread_b;
tss_t storage_a;
tss_t storage_b;


void arrays()
{
  cnd_t conditions[] = {condition_a, condition_b}; // UndCC_Violation(Win)
  mtx_t mutexes[] = {mutex_a, mutex_b};            // UndCC_Violation(Win)
  thrd_t threads[] = {thread_a, thread_b};         // UndCC_Violation(Win)
  tss_t storages[] = {storage_a, storage_b};       // UndCC_Violation(Win)
}


void conditions()
{
  cnd_broadcast(&condition_a);
  cnd_destroy(&condition_a);
  cnd_init(&condition_a);
  cnd_signal(&condition_a);
  cnd_timedwait(&condition_a, &mutex_a, 0);
  cnd_wait(&condition_a, &mutex_a);
}


void mutexes()
{
  mtx_destroy(&mutex_a);
  mtx_init(&mutex_a, mtx_plain);
  mtx_lock(&mutex_a);
  mtx_timedlock(&mutex_a, 0);
  mtx_trylock(&mutex_a);
  mtx_unlock(&mutex_a);
}


void threads()
{
  thrd_create(&thread_a, 0, 0);
  thrd_current();
  thrd_detach(thread_a);
  thrd_equal(thread_a, thread_b);
  thrd_exit(0);
  thrd_join(thread_a, 0);
  thrd_sleep(0, 0);
  thrd_yield();
}


void storages()
{
  tss_create(&storage_a, 0);
  tss_delete(storage_a);
  tss_get(storage_a);
  tss_set(storage_a, 0);
}
