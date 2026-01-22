/* MISRA C 2004 */


#include <stdint.h>


void non_compliant()
{
  struct { int16_t key; int16_t value; } record; /* UndCC_Violation */
  int16_t value;  /* UndCC_Violation - second use of value */

  record.key = 1;
  value = 0;      /* should have been record.value */
}


void compliant()
{
  #define N_DEVICES 32
  #define N_TASKS 32

  struct device_q { struct device_q *next;   /* ... */ }
  devices[N_DEVICES];
  struct task_q { struct task_q *next;      /* ... */ }
  tasks[N_TASKS];

  devices[0].next = &devices[1];
  tasks[0].next   = &tasks[1];
}
