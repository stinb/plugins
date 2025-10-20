// Custom


#include <stdatomic.h>
#include <stdint.h>


void main( void )
{
  _Atomic uint8_t atom;
  uint8_t normal;
  atomic_init(&atom, normal);

  normal = atomic_load_explicit(&atom, memory_order_relaxed); // UndCC_Violation
  normal = atomic_load_explicit(&atom, memory_order_consume); // UndCC_Violation
  normal = atomic_load_explicit(&atom, memory_order_acquire); // UndCC_Violation
  normal = atomic_load_explicit(&atom, memory_order_release); // UndCC_Violation
  normal = atomic_load_explicit(&atom, memory_order_acq_rel); // UndCC_Violation
  normal = atomic_load_explicit(&atom, memory_order_seq_cst); // UndCC_Valid

  const memory_order RELAXED = memory_order_relaxed;
  const memory_order CONSUME = memory_order_consume;
  const memory_order ACQUIRE = memory_order_acquire;
  const memory_order RELEASE = memory_order_release;
  const memory_order ACQ_REL = memory_order_acq_rel;
  const memory_order SEQ_CST = memory_order_seq_cst;

  atomic_compare_exchange_strong_explicit(&atom, &normal, normal, RELAXED, SEQ_CST); // UndCC_Violation
  atomic_compare_exchange_strong_explicit(&atom, &normal, normal, CONSUME, SEQ_CST); // UndCC_Violation
  atomic_compare_exchange_strong_explicit(&atom, &normal, normal, ACQUIRE, SEQ_CST); // UndCC_Violation
  atomic_compare_exchange_strong_explicit(&atom, &normal, normal, SEQ_CST, RELEASE); // UndCC_Violation
  atomic_compare_exchange_strong_explicit(&atom, &normal, normal, SEQ_CST, ACQ_REL); // UndCC_Violation
  atomic_compare_exchange_strong_explicit(&atom, &normal, normal, SEQ_CST, SEQ_CST); // UndCC_Valid
}
