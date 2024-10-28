#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>

void
exchange (atomic_int *i)
{
  int r;

  atomic_compare_exchange_strong_explicit (i, &r, 0, memory_order_seq_cst, memory_order_release);
  atomic_compare_exchange_strong_explicit (i, &r, 0, memory_order_seq_cst, memory_order_acq_rel);
  atomic_compare_exchange_strong_explicit (i, &r, 0, memory_order_relaxed, memory_order_consume);

  atomic_compare_exchange_weak_explicit (i, &r, 0, memory_order_seq_cst, memory_order_release); /* UndCC_Violation */
  atomic_compare_exchange_weak_explicit (i, &r, 0, memory_order_seq_cst, memory_order_acq_rel); /* UndCC_Violation */
  atomic_compare_exchange_weak_explicit (i, &r, 0, memory_order_relaxed, memory_order_consume); /* UndCC_Violation */
}
