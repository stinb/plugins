#include "posix_typedefs.h"

typedef struct vector { uint16_t x; uint16_t y; uint16_t z; } vector; // UndCC_Violation
/* Rule violation ^^                                             ^^ */

struct { int16_t key; int16_t value; } record; // UndCC_Violation
void func1() {
	int16_t value; /* Rule violation - second use of value */ // UndCC_Violation
	record.key = 1;
	value = 0;      /* should have been record.value */
}

/* no violations here */
#define N_DEVICES 20
#define N_TASKS 20
struct device_q { struct device_q *next;  /* ... */ } devices[N_DEVICES];
struct task_q { struct task_q *next;     /* ... */ } tasks[N_TASKS];
void func2() {
	devices[0].next = &devices[1];
	tasks[0].next   = &tasks[1];
}
