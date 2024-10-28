// SEI CERT C

#if __has_include(<pthread.h>)

#include <pthread.h>
#include <stdlib.h>

typedef struct {
  int balance;
  pthread_mutex_t balance_mutex;
  unsigned int id; /* Should never be changed after initialized */
} bank_account;

typedef struct {
  bank_account *from;
  bank_account *to;
  int amount;
} deposit_thr_args;

unsigned int global_id = 1;

void create_bank_account(bank_account **ba, int initial_amount) {
  int result;
  bank_account *nba = malloc(sizeof(bank_account));
  if (nba == NULL) {
    /* Handle error */
  }

  nba->balance = initial_amount;
  result = pthread_mutex_init(&nba->balance_mutex, NULL);
  if (result != 0) {
    /* Handle error */
  }

  nba->id = global_id++;
  *ba = nba;
}

void *deposit(void *ptr) {
  deposit_thr_args *args = (deposit_thr_args *)ptr;
  int result;

  if (args->from->id == args->to->id)
    return;

  /* Ensure proper ordering for locking */
  if (args->from->id < args->to->id) {
    if ((result = pthread_mutex_lock(&(args->from->balance_mutex))) != 0) { // UndCC_Valid
      /* Handle error */
    }
    if ((result = pthread_mutex_lock(&(args->to->balance_mutex))) != 0) { // UndCC_Valid
      /* Handle error */
    }
  } else {
    if ((result = pthread_mutex_lock(&(args->to->balance_mutex))) != 0) { // UndCC_Valid
      /* Handle error */
    }
    if ((result = pthread_mutex_lock(&(args->from->balance_mutex))) != 0) { // UndCC_Valid
      /* Handle error */
    }
  }

  /* Not enough balance to transfer */
  if (args->from->balance < args->amount) {
    if ((result = pthread_mutex_unlock(&(args->from->balance_mutex))) != 0) {
      /* Handle error */
    }
    if ((result = pthread_mutex_unlock(&(args->to->balance_mutex))) != 0) {
      /* Handle error */
    }
    return;
  }

  args->from->balance -= args->amount;
  args->to->balance += args->amount;

  if ((result = pthread_mutex_unlock(&(args->from->balance_mutex))) != 0) {
    /* Handle error */
  }
  if ((result = pthread_mutex_unlock(&(args->to->balance_mutex))) != 0) {
    /* Handle error */
  }

  free(ptr);
  return;
}

#endif // __has_include(<pthread.h>)
