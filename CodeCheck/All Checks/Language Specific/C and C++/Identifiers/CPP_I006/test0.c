// MISRA C 2012

#include <stdint.h>

#define Sum(x, y) ( ( x ) + ( y ) )
int16_t Sum;                                        // UndCC_Violation

#define Sum(x, y) ( ( x ) + ( y ) )
int16_t x = Sum ( 1, 2 );                           // UndCC_Valid

/*              1234567890123456789012345678901********* Characters */
#define         low_pressure_turbine_temperature_1 lp_tb_temp_1
static int32_t  low_pressure_turbine_temperature_2; // UndCC_Violation
