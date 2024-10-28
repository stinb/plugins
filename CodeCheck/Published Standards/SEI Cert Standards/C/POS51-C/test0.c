// SEI CERT C

#if __has_include(<pthread.h>)

#include <pthread.h>
#include <stdlib.h>

typedef struct {
  int balance;
  pthread_mutex_t balance_mutex;
} bank_account;

typedef struct {
  bank_account *from;
  bank_account *to;
  int amount;
} deposit_thr_args;

void create_bank_account(bank_account **ba, int initial_amount) {
  int result;
  bank_account *nba = malloc(sizeof(bank_account));
  if (nba == NULL) {
    /* Handle error */
  }

  nba->balance = initial_amount;
  result = pthread_mutex_init(&nba->balance_mutex, NULL);
  if (result) {
    /* Handle error */
  }

  *ba = nba;
}


void *deposit(void *ptr) {
  int result;
  deposit_thr_args *args = (deposit_thr_args *)ptr;

  if ((result = pthread_mutex_lock(&(args->from->balance_mutex))) != 0) { // UndCC_Violation(lin,mac)
    /* Handle error */
  }

  /* Not enough balance to transfer */
  if (args->from->balance < args->amount) {
    if ((result = pthread_mutex_unlock(&(args->from->balance_mutex))) != 0) {
      /* Handle error  */
    }
    return NULL;
  }

  if ((result = pthread_mutex_lock(&(args->to->balance_mutex))) != 0) { // UndCC_Violation(lin,mac)
    /* Handle error */
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
  return NULL;
}

int main(void) {

  pthread_t thr1, thr2;
  int result;

  bank_account *ba1;
  bank_account *ba2;
  create_bank_account(&ba1, 1000);
  create_bank_account(&ba2, 1000);

  deposit_thr_args *arg1 = malloc(sizeof(deposit_thr_args));
  if (arg1 == NULL) {
    /* Handle error */
  }
  deposit_thr_args *arg2 = malloc(sizeof(deposit_thr_args));
  if (arg2 == NULL) {
    /* Handle error */
  }

  arg1->from = ba1;
  arg1->to = ba2;
  arg1->amount = 100;

  arg2->from = ba2;
  arg2->to = ba1;
  arg2->amount = 100;

  /* Perform the deposits */
  if ((result = pthread_create(&thr1, NULL, deposit, (void *)arg1)) != 0) {
    /* Handle error */
  }
  if ((result = pthread_create(&thr2, NULL, deposit, (void *)arg2)) != 0) {
    /* Handle error */
  }

  pthread_exit(NULL);
  return 0;
}

#endif // __has_include(<pthread.h>)
