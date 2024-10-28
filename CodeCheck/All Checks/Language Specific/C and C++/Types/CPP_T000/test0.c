#include "test1.h"

// MISRA C 2012

/* Non-compliant - int used to define an object                      */
int x = 0;                // UndCC_Violation

/* Compliant     - int used to define specific-length type           */
typedef int SINT_16;      // UndCC_Valid

/* Non-compliant - no sign or size specified                         */
typedef int speed_t;      // UndCC_Violation

/* Compliant     - further abstraction does not need specific length */
typedef int16_t torque_t; // UndCC_Valid
