/* MISRA C 2012 & 2023 */

#include <stdint.h>

void unusedtag ( void )
{
  enum state { S_init, S_run, S_sleep };   /* UndCC_Violation */
}

typedef struct record_t                    /* UndCC_Violation */
{
  uint16_t  key;
  uint16_t  val;
} record1_t;

typedef struct                             /* UndCC_Valid */
{
  uint16_t  key;
  uint16_t  val;
} record2_t;
