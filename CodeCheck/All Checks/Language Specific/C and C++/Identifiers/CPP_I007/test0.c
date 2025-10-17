/* MISRA C 2012 */

#include <stdint.h>

/*             1234567890123456789012345678901*********        Characters */
extern int32_t engine_exhaust_gas_temperature_raw;
static int32_t engine_exhaust_gas_temperature_scaled;  /* UndCC_Violation */

void f ( void )
{
  /*      1234567890123456789012345678901*********             Characters */
  int32_t engine_exhaust_gas_temperature_local;        /* UndCC_Valid     */
}

/*             1234567890123456789012345678901*********        Characters */
static int32_t engine_exhaust_gas_temp_raw;
static int32_t engine_exhaust_gas_temp_scaled;         /* UndCC_Valid     */

int x = 10;
