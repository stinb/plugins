#define F1_MACRO

#include <stdint.h>
#include "20.1includesComeFirst.h" /* UndCC_Valid */
#include <stdio.h> /* UndCC_Valid */

int32_t i = 0;

#include "f3.h" /* UndCC_Violation */
