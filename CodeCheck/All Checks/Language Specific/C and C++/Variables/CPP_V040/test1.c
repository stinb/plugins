// Custom


#include <threads.h>


typedef struct
{
  mtx_t mutex;
  cnd_t condition;
} MutexAndCondition;

MutexAndCondition a;
MutexAndCondition b;
MutexAndCondition c;


void wait(MutexAndCondition *mc)
{
  mtx_lock(&mc->mutex);
  cnd_wait(&mc->condition, &mc->mutex); // UndCC_Valid
  mtx_unlock(&mc->mutex);
}


void wait_a()
{
  mtx_lock(&a.mutex);
  cnd_wait(&a.condition, &a.mutex); // UndCC_Valid
  mtx_unlock(&a.mutex);
}


void wait_b_mistake_1()
{
  mtx_lock(&b.mutex);
  cnd_wait(&b.condition, &b.mutex); // UndCC_Violation
  mtx_unlock(&b.mutex);
}


void wait_b_mistake_2()
{
  mtx_lock(&b.mutex);
  cnd_wait(&b.condition, &c.mutex); // UndCC_Violation
  mtx_unlock(&b.mutex);
}
